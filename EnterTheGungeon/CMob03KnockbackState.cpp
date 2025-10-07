#pragma region INCLUDE
#include "pch.h"
#include "CMob03KnockbackState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#include "CSoundManager.h"
#pragma endregion

CMob03KnockbackState::CMob03KnockbackState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob03KnockbackState::Initialize()
{
	CMobState::Initialize();

	fCurStateMaxTime = 0.25f;

	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_HIT.bmp", L"Monster03_HIT");
	animation.Initialize(0, 0, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMob03KnockbackState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
}

void CMob03KnockbackState::Late_Update()
{
	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		pStateMachine->Change_State(CMob::Idle);
	}
}

void CMob03KnockbackState::Render(HDC hDC)
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

void CMob03KnockbackState::Release()
{
}

void CMob03KnockbackState::Exit()
{
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(0.1f);
}

void CMob03KnockbackState::Enter()
{
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"beholster_hurt_01.wav", 1.f);
	// MANAGER(CSoundManager*, M_SOUND)->PlaySoundW(L"beholster_hurt_01.wav", SOUND_EFFECT, 1.f);
	dwCurrentStateElapsedTime = GetTickCount();
	Detect_Player();
	pObj->Get_Transform()->Direction(vDirToPlayer * -1.f);
	static_cast<CMob*>(pObj)->Set_Speed(3.f); // original is 1.5f
}

void CMob03KnockbackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob03KnockbackState::Stop_Animation()
{
}

void CMob03KnockbackState::On_End_Animation()
{
}

int CMob03KnockbackState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}
