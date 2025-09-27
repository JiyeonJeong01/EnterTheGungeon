#include "pch.h"
#include "CMob04DeadState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob04.h"
#include "CStateMachine.h"
#include "CStageManager.h"

CMob04DeadState::CMob04DeadState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob04DeadState::Initialize()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster04_DEAD.bmp", L"Monster04_DEAD");

	CMobState::Initialize();

	fCurStateMaxTime = 3.f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Dead;

	animation.Initialize(0, 4, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 100);
}

void CMob04DeadState::Update()
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

void CMob04DeadState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster04_DEAD");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 150;
	int renderSize = 110;
	
	if (bCanMoveAnim)
		iDeadDir = Dir_AnimRow(eDir);
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		renderSize, renderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		iDeadDir * realSize,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob04DeadState::Exit()
{
}

void CMob04DeadState::Enter()
{
	bCanMoveAnim = true;
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	dwCurrentStateElapsedTime = GetTickCount();
	MANAGER(CStageManager*, M_STAGE)->OnKilled_Enemy(static_cast<CMob*>(pObj));
}

void CMob04DeadState::On_End_Animation()
{
	bCanMoveAnim = false;
}

int CMob04DeadState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
