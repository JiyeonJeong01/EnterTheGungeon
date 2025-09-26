#include "pch.h"
#include "CMob01IdleState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob01.h"
#include "CStateMachine.h"
#include "CMob01StateMachine.h"

CMob01IdleState::CMob01IdleState(CObject* pObj, CStateMachine* pStateMachine) 
	: CMobState(pObj, pStateMachine)
{

}

void CMob01IdleState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 1.5f; 
	dwLastAttackTime = GetTickCount();
	fCurStateMaxTime = 1.5f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster01_IDLE.bmp", L"Monster01_IDLE");
	animation.Initialize(0, 2, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMob01IdleState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob01IdleState::Late_Update()
{
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount()
		&& (fCurDistToPlayer <= fMinStopDist || fCurDistToPlayer >= fMaxStopDist))
	{
		pStateMachine->Change_State(CMob::Walk);
	}
}

void CMob01IdleState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster01_IDLE");

	CRenderer renderer = *(pObj->Get_Renderer());
	int size = 220;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		80, 80,
		hMemDC,
		animation.iCurrIndex * size,
		Dir_AnimRow(eDir) * size,
		size, size,
		RGB(255, 0, 255));
}

void CMob01IdleState::Release()
{
}

void CMob01IdleState::Exit()
{
}

void CMob01IdleState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CMob01IdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob01IdleState::Stop_Animation()
{
}

void CMob01IdleState::On_End_Animation()
{
}

int CMob01IdleState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: case D_UL: case D_UR:
		return 0;
	case D_DOWN: case D_DL: case D_LEFT:
		return 1;
	case D_RIGHT: case D_DR:
		return 2;
	case D_END:
		break;
	default:
		break;
	}



	return 0;
}
