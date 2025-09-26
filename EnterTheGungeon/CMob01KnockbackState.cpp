#pragma region INCLUDE
#include "pch.h"
#include "CMob01KnockbackState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob01.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#pragma endregion

CMob01KnockbackState::CMob01KnockbackState(CObject* pObj, CStateMachine* pStateMachine) 
	: CMobState(pObj, pStateMachine)
{
}

void CMob01KnockbackState::Initialize()
{
	CMobState::Initialize();

	fCurStateMaxTime = 0.25f;

	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster01_HIT.bmp", L"Monster01_HIT");
	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMob01KnockbackState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob01KnockbackState::Late_Update()
{
	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		pStateMachine->Change_State(CMob::Idle);
	}
}

void CMob01KnockbackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster01_HIT");

	CRenderer renderer = *(pObj->Get_Renderer());
	int size = 220;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		80, 80,
		hMemDC,
		animation.iCurrIndex * size,
		Dir_AnimRow(eDir) * size,
		size, size,
		RGB(255, 0, 255));
}

void CMob01KnockbackState::Release()
{
}

void CMob01KnockbackState::Exit()
{
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(1.5f);
}

void CMob01KnockbackState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	Detect_Player();
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(10.f); // original is 1.5f
}

void CMob01KnockbackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob01KnockbackState::Stop_Animation()
{
}

void CMob01KnockbackState::On_End_Animation()
{
}

int CMob01KnockbackState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
