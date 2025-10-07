#pragma region INCLUDE
#include "pch.h"
#include "CMob04KnockbackState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob04.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#include "CSoundManager.h"
#pragma endregion

CMob04KnockbackState::CMob04KnockbackState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob04KnockbackState::Initialize()
{
	CMobState::Initialize();

	fCurStateMaxTime = 0.25f;

	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster04_HIT.bmp", L"Monster04_HIT");

	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMob04KnockbackState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob04KnockbackState::Late_Update()
{
	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		pStateMachine->Change_State(CMob::Idle);
	}
}

void CMob04KnockbackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster04_HIT");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 150;
	int renderSize = 110;

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		renderSize, renderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		Dir_AnimRow(eDir) * realSize,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob04KnockbackState::Release()
{
}

void CMob04KnockbackState::Exit()
{
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(1.5f);
}

void CMob04KnockbackState::Enter()
{
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"rubber_hurt_01.wav", 1.f);

	dwCurrentStateElapsedTime = GetTickCount();
	Detect_Player();
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(10.f); // original is 1.5f
}

void CMob04KnockbackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob04KnockbackState::Stop_Animation()
{
}

void CMob04KnockbackState::On_End_Animation()
{
}

int CMob04KnockbackState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
