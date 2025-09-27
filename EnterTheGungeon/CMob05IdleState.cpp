#include "pch.h"
#include "CMob05IdleState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob05.h"
#include "CStateMachine.h"
#include "CMob01StateMachine.h"

CMob05IdleState::CMob05IdleState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{

}

void CMob05IdleState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 1.5f;
	dwLastAttackTime = GetTickCount();
	fCurStateMaxTime = 1.5f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster05_IDLE.bmp", L"Monster05_IDLE");
	animation.Initialize(0, 2, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMob05IdleState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob05IdleState::Late_Update()
{
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount()
		|| (fCurDistToPlayer <= fMinStopDist || fCurDistToPlayer >= fMaxStopDist))
	{
		pStateMachine->Change_State(CMob::Walk);
	}
}

void CMob05IdleState::Render(HDC hDC)
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

void CMob05IdleState::Release()
{
}

void CMob05IdleState::Exit()
{
}

void CMob05IdleState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CMob05IdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob05IdleState::Stop_Animation()
{
}

void CMob05IdleState::On_End_Animation()
{
}

int CMob05IdleState::Dir_AnimRow(Direction eDir)
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
