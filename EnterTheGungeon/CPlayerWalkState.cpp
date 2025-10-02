#include "pch.h"
#include "CPlayerWalkState.h"
#include "CTransform.h"
#include "CManager.h"
#include "CBmpManager.h"
#include "CInputManager.h"
#include "CRenderer.h"
#include "CStateMachine.h"
#include "CCameraManager.h"
#include "CSoundManager.h"
#include "CPlayerWeapon.h"

void CPlayerWalkState::Initialize()
{
	eState = CPlayer::PS_IDLE;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Player/Player_WALK.bmp", L"Player_WALK");
	animation.Initialize(0, 5, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 110);
}

void CPlayerWalkState::Update()
{
	CPlayerState::Update();
	Update_AnimFrame();

	if (vInputDir.X() == 0.f && vInputDir.Y() == 0.f)
	{
		pStateMachine->Change_State((CPlayer::PS_IDLE));
		return;
	}
	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RBUTTON))
	{
		pStateMachine->Change_State((CPlayer::PS_DODGE));
		return;
	}

	Vector2 vNewPos = {
		pObj->Get_Transform()->Position().X() + vInputDir.X() * fSpeed,
		pObj->Get_Transform()->Position().Y() + vInputDir.Y() * fSpeed,
	};
	pObj->Get_Transform()->Position(move(vNewPos));
}

void CPlayerWalkState::Late_Update()
{
}

void CPlayerWalkState::Render(HDC hDC)
{
	CPlayerState::Render_Player(hDC);


	CWeapon::WeaponType wType = static_cast<CPlayer*>(pObj)->pWeapon->Get_WeaponType();
	const TCHAR* animKey = (wType == CWeapon::PG01) ? L"Player_Weapon01" : L"Player_Weapon02";
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Player_WALK");
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
			renderer.Top() + Get_WeaponPos() + 15 + iShotOffsetY,
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

void CPlayerWalkState::Release()
{
}

void CPlayerWalkState::Exit()
{
}

void CPlayerWalkState::Enter()
{
}

void CPlayerWalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
	if (iPrevFrame != animation.iCurrIndex)
	{
		iPrevFrame = animation.iCurrIndex;
		MANAGER(CSoundManager*, M_SOUND)->StopSound(SOUND_EFFECT);
		MANAGER(CSoundManager*, M_SOUND)->PlaySoundW(L"Player_FootStep.wav", SOUND_EFFECT, 1.f);
	}
}

void CPlayerWalkState::Stop_Animation()
{
}

void CPlayerWalkState::On_End_Animation()
{
}

int CPlayerWalkState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: return 0;
	case D_UL: return 1;
	case D_UR: return 2;
	case D_DOWN: return 3;
	case D_LEFT: case D_DL: return 4;
	case D_RIGHT: case D_DR: return 5;
	}
	return 0;
}

int CPlayerWalkState::Get_WeaponPos()
{
	switch (animation.iCurrIndex)
	{
	case 0: case 3: case 5:
		return 0;
	case 1: case 2: case 4:
		return 5;
	}
	return 0;
}