#include "pch.h"
#include "CMob03IdleState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"

CMob03IdleState::CMob03IdleState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{

}

void CMob03IdleState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 1.5f;
	dwLastAttackTime = GetTickCount();
	fCurStateMaxTime = 1.5f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_IDLE.bmp", L"Monster03_IDLE");
	animation.Initialize(0, 1, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMob03IdleState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob03IdleState::Late_Update()
{
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount()
		|| (fCurDistToPlayer <= fMinStopDist || fCurDistToPlayer >= fMaxStopDist))
	{
		pStateMachine->Change_State(CMob::Walk);
	}
}

void CMob03IdleState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster03_IDLE");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 230;
	int iRenderSize = 230;

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		iRenderSize, iRenderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		0,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob03IdleState::Release()
{
}

void CMob03IdleState::Exit()
{
}

void CMob03IdleState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CMob03IdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob03IdleState::Stop_Animation()
{
}

void CMob03IdleState::On_End_Animation()
{
}

int CMob03IdleState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() > 0) return 0;
	else return 1;
}
