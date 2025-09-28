#include "pch.h"
#include "CMobOwnerDeadState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"
#include "CStageManager.h"

CMobOwnerDeadState::CMobOwnerDeadState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMobOwnerDeadState::Initialize()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_DEAD.bmp", L"Monster02_DEAD");

	CMobState::Initialize();

	fCurStateMaxTime = 3.f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Dead;

	animation.Initialize(0, 2, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMobOwnerDeadState::Update()
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

void CMobOwnerDeadState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster03_DEAD");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 230;
	int iRenderSize = 230;


	//GdiTransparentBlt(hDC,
	//	renderer.Left(),
	//	renderer.Top(),
	//	iRenderSize, iRenderSize,
	//	hMemDC,
	//	animation.iCurrIndex * realSize,
	//	0,
	//	realSize, realSize,
	//	RGB(255, 0, 255));
	// 
	GdiTransparentBlt(hDC,
		0, 0,
		iRenderSize, iRenderSize,
		hMemDC,
		0,
		0,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMobOwnerDeadState::Exit()
{
}

void CMobOwnerDeadState::Enter()
{
	bCanMoveAnim = true;
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	dwCurrentStateElapsedTime = GetTickCount();
	MANAGER(CStageManager*, M_STAGE)->OnKilled_Enemy(static_cast<CMob*>(pObj));
}

void CMobOwnerDeadState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMobOwnerDeadState::On_End_Animation()
{
}

int CMobOwnerDeadState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
