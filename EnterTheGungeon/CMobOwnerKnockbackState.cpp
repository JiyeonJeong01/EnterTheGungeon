#pragma region INCLUDE
#include "pch.h"
#include "CMobOwnerKnockbackState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#pragma endregion

CMobOwnerKnockbackState::CMobOwnerKnockbackState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMobOwnerKnockbackState::Initialize()
{
	CMobState::Initialize();

	fCurStateMaxTime = 0.25f;

	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_HIT.bmp", L"Monster03_HIT");
	animation.Initialize(0, 0, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMobOwnerKnockbackState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMobOwnerKnockbackState::Late_Update()
{
	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		pStateMachine->Change_State(CMob::Idle);
	}
}

void CMobOwnerKnockbackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster03_HIT");

	CRenderer renderer = *(pObj->Get_Renderer());
	int realSize = 230;
	int iRenderSize = 230;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		iRenderSize, iRenderSize,
		hMemDC,
		0,
		0,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMobOwnerKnockbackState::Release()
{
}

void CMobOwnerKnockbackState::Exit()
{
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(0.1f);
}

void CMobOwnerKnockbackState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	Detect_Player();
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(3.f); // original is 1.5f
}

void CMobOwnerKnockbackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMobOwnerKnockbackState::Stop_Animation()
{
}

void CMobOwnerKnockbackState::On_End_Animation()
{
}

int CMobOwnerKnockbackState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
