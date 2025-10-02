#pragma region INCLUDE
#include "pch.h"
#include "CPlayerIdleState.h"
#include "CManager.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CStateMachine.h"
#include "CPlayerWeapon.h"
#pragma endregion

void CPlayerIdleState::Initialize()
{
	eState = CPlayer::PS_IDLE;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Player/Player_IDLE.bmp", L"Player_IDLE");

	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CPlayerIdleState::Update()
{
	CPlayerState::Update();

	Update_AnimFrame();
}

void CPlayerIdleState::Late_Update()
{
	if (vInputDir.X() != 0.f || vInputDir.Y() != 0.f)
	{
		pStateMachine->Change_State(CPlayer::PS_WALK);
	}
}

void CPlayerIdleState::Render(HDC hDC)
{
	CPlayerState::Render_Player(hDC);
	CWeapon::WeaponType wType = static_cast<CPlayer*>(pObj)->pWeapon->Get_WeaponType();
	const TCHAR* animKey = (wType == CWeapon::PG01) ? L"Player_Weapon01" : L"Player_Weapon02";
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Player_IDLE");
	HDC hWeaponDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(animKey);

	CRenderer renderer = *(pObj->Get_Renderer());

	int iGunLeftPos = (iWeaponRowIndex == 0) ? 30 : -40;
	int iShotLeftPos = (iWeaponRowIndex == 0) ? 60 : -10;

	GdiTransparentBlt(hDC,
		renderer.Left() + iGunLeftPos,
		renderer.Top() + Get_WeaponPos(),
		(int)73,
		(int)90,
		hWeaponDC,
		iWeaponColIndex * 73, iWeaponRowIndex * 90,
		(int)73,
		(int)90,
		RGB(30, 30, 30));

	if (bShouldShotEffect && !bEndShotEffect)
	{
		HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Shot");

		if (dwShotAnimElapsedTime + 200 < GetTickCount())
		{
			iShotCol++;
			if (iShotCol > 2)
			{
				bEndShotEffect = true;
			}
		}

		GdiTransparentBlt(hDC,
			renderer.Left() + iGunLeftPos + iShotLeftPos,
			renderer.Top() + Get_WeaponPos()  +iShotOffsetY,
			iShotEffectSizeX * 0.8f, iShotEffectSizeY * 0.8f, 
			hMemDC,
			iShotCol * iShotEffectSizeX, 0,
			iShotEffectSizeX, iShotEffectSizeY, 
			RGB(32, 32, 32));
	}

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)renderer.Size().X(),
		Dir_AnimRow(eDir) * (int)renderer.Size().Y(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		RGB(255, 0, 255));
}

void CPlayerIdleState::Release()
{
}

void CPlayerIdleState::Exit()
{
}

void CPlayerIdleState::Enter()
{
	vInputDir = { 0.f, 0.f };
}

void CPlayerIdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CPlayerIdleState::Stop_Animation()
{
}

void CPlayerIdleState::On_End_Animation()
{
}

int CPlayerIdleState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP:  return 0;
	case D_UL: return 1;
	case D_UR: return 2;
	case D_DOWN: return 3;
	case D_LEFT: case D_DL: return 4;
	case D_RIGHT: case D_DR: return 5;
	}
	return 0;
}

int CPlayerIdleState::Get_WeaponPos()
{
	switch (animation.iCurrIndex)
	{
	case 0: case 3:
		return 0;
	case 1: case 2:
		return 5;
	}
	return 0;
}

int CPlayerIdleState::Get_WeaponDir()
{


	return 0;
}
