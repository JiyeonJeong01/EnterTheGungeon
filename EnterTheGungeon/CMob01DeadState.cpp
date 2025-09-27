#include "pch.h"
#include "CMob01DeadState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob01.h"
#include "CStateMachine.h"
#include "CStageManager.h"

CMob01DeadState::CMob01DeadState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob01DeadState::Initialize()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster01_DEAD.bmp", L"Monster01_DEAD");

	CMobState::Initialize();

	fCurStateMaxTime = 3.f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Dead;
	
	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 100);
}

void CMob01DeadState::Update()
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

void CMob01DeadState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster01_DEAD");

	CRenderer renderer = *(pObj->Get_Renderer());
	int size = 220;
	if (bCanMoveAnim)	
		iDeadDir = Dir_AnimRow(eDir);
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		80, 80,
		hMemDC,
		animation.iCurrIndex * size,
		iDeadDir * size,
		size, size,
		RGB(255, 0, 255));
}

void CMob01DeadState::Exit()
{
}

void CMob01DeadState::Enter()
{
	bCanMoveAnim = true;
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	dwCurrentStateElapsedTime = GetTickCount();
}

void CMob01DeadState::On_End_Animation()
{
	bCanMoveAnim = false;
	MANAGER(CStageManager*, M_STAGE)->OnKilled_Enemy(static_cast<CMob*>(pObj));
}

int CMob01DeadState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
