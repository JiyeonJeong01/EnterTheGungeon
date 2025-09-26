#include "pch.h"
#include "CMob02IdleState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob02.h"
#include "CStateMachine.h"

CMob02IdleState::CMob02IdleState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{

}

void CMob02IdleState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 1.5f;
	dwLastAttackTime = GetTickCount();
	fCurStateMaxTime = 1.5f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster02_IDLE.bmp", L"Monster02_IDLE");
	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMob02IdleState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob02IdleState::Late_Update()
{
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount()
		|| (fCurDistToPlayer <= fMinStopDist || fCurDistToPlayer >= fMaxStopDist))
	{
		pStateMachine->Change_State(CMob::Walk);
	}
}

void CMob02IdleState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster02_IDLE");

	CRenderer renderer = *(pObj->Get_Renderer());
	int size = 400;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		100, 100,
		hMemDC,
		animation.iCurrIndex * size,
		Dir_AnimRow(eDir) * size,
		size, size,
		RGB(255, 0, 255));
}

void CMob02IdleState::Release()
{
}

void CMob02IdleState::Exit()
{
}

void CMob02IdleState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CMob02IdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob02IdleState::Stop_Animation()
{
}

void CMob02IdleState::On_End_Animation()
{
}

int CMob02IdleState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() > 0) return 0;
	else return 1;
}
