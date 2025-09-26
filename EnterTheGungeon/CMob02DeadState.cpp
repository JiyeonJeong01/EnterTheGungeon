#include "pch.h"
#include "CMob02DeadState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob02.h"
#include "CStateMachine.h"

CMob02DeadState::CMob02DeadState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob02DeadState::Initialize()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster02_DEAD.bmp", L"Monster02_DEAD");

	CMobState::Initialize();

	fCurStateMaxTime = 3.f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Dead;

	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 80);
}

void CMob02DeadState::Update()
{
	CMobState::Update();

	if (bCanMoveAnim)
		Update_AnimFrame();
	else
	{
		animation.iCurrIndex = animation.iEndIndex;
	}

	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		static_cast<CMob*>(pObj)->Set_Dead();
	}
}

void CMob02DeadState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster02_DEAD");

	CRenderer renderer = *(pObj->Get_Renderer());
	int size = 400;
	if (bCanMoveAnim)
	{
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

}

void CMob02DeadState::Exit()
{
}

void CMob02DeadState::Enter()
{
	bCanMoveAnim = true;
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	dwCurrentStateElapsedTime = GetTickCount();
}

void CMob02DeadState::On_End_Animation()
{
	bCanMoveAnim = false;

}

int CMob02DeadState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
