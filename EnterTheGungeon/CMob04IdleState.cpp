#include "pch.h"
#include "CMob04IdleState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob04.h"
#include "CStateMachine.h"
#include "CMob01StateMachine.h"

CMob04IdleState::CMob04IdleState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{

}

void CMob04IdleState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 1.5f;
	dwLastAttackTime = GetTickCount();
	fCurStateMaxTime = 1.5f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster04_IDLE.bmp", L"Monster04_IDLE");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster05_IDLE.bmp", L"Monster05_IDLE");
	animation.Initialize(0, 2, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMob04IdleState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob04IdleState::Late_Update()
{
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount()
		|| (fCurDistToPlayer <= fMinStopDist || fCurDistToPlayer >= fMaxStopDist))
	{
		pStateMachine->Change_State(CMob::Walk);
	}
}

void CMob04IdleState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster05_IDLE");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 150;
	int renderSize = 110;	
	
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		renderSize, renderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		Dir_AnimRow(eDir) * realSize,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob04IdleState::Release()
{
}

void CMob04IdleState::Exit()
{
}

void CMob04IdleState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CMob04IdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob04IdleState::Stop_Animation()
{
}

void CMob04IdleState::On_End_Animation()
{
}

int CMob04IdleState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: case D_UL: case D_UR:
		return 0;
	default:
		return 1;
		break;
	}

	return 0;
}
