#pragma region INCLUDE
#include "pch.h"
#include "CMob02KnockbackState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob02.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#pragma endregion

CMob02KnockbackState::CMob02KnockbackState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob02KnockbackState::Initialize()
{
	CMobState::Initialize();

	fCurStateMaxTime = 0.25f;

	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster02_HIT.bmp", L"Monster02_HIT");
	animation.Initialize(0, 0, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMob02KnockbackState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob02KnockbackState::Late_Update()
{
	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		pStateMachine->Change_State(CMob::Idle);
	}
}

void CMob02KnockbackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster02_HIT");

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

void CMob02KnockbackState::Release()
{
}

void CMob02KnockbackState::Exit()
{
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(1.5f);
}

void CMob02KnockbackState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	Detect_Player();
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(10.f); // original is 1.5f
}

void CMob02KnockbackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob02KnockbackState::Stop_Animation()
{
}

void CMob02KnockbackState::On_End_Animation()
{
}

int CMob02KnockbackState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
