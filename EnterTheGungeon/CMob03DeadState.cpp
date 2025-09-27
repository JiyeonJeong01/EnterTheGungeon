#include "pch.h"
#include "CMob03DeadState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"

CMob03DeadState::CMob03DeadState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob03DeadState::Initialize()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_DEAD.bmp", L"Monster02_DEAD");

	CMobState::Initialize();

	fCurStateMaxTime = 3.f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Dead;

	animation.Initialize(0, 2, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMob03DeadState::Update()
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

void CMob03DeadState::Render(HDC hDC)
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

void CMob03DeadState::Exit()
{
}

void CMob03DeadState::Enter()
{
	bCanMoveAnim = true;
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	dwCurrentStateElapsedTime = GetTickCount();
}

void CMob03DeadState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob03DeadState::On_End_Animation()
{
}

int CMob03DeadState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
