#pragma region INCLUDE
#include "pch.h"
#include "CPlayerDodgeState.h"
#include "CManager.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CStateMachine.h"
#pragma endregion


void CPlayerDodgeState::Initialize()
{
	eState = CPlayer::PS_DODGE;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Player/Player_DODGE.bmp", L"Player_DODGE");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Player/Player_Effect_DODGE.bmp", L"Player_Effect_DODGE");

	animation.Initialize(0, 8, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 40.f);
	vDodgeDir = { 0.f, 0.f };

	effectAnim.Initialize(0, 5, 0);
	fill(effectAnim.vTransitTime.begin(), effectAnim.vTransitTime.end(), 40);
	bEffectPlay = false;
}

void CPlayerDodgeState::Update()
{
	CPlayerState::Update();

	Update_AnimFrame();
	if (!bDodgePlaying)
	{
		bDodgePlaying = true;
		Vector2 vNewPos = {
		pObj->Get_Transform()->Position().X() + vDodgeDir.X() * fSpeed * 1.5f,
		pObj->Get_Transform()->Position().Y() + vDodgeDir.Y() * fSpeed * 1.5f,
		};
		pObj->Get_Transform()->Position(move(vNewPos));
		Update_AnimFrame();
	}

}

void CPlayerDodgeState::Late_Update()
{
}

void CPlayerDodgeState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Player_DODGE");
	HDC hMemDC2 = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Player_Effect_DODGE");

	CRenderer renderer = *(pObj->Get_Renderer());

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

	if (bEffectPlay)
	{
		GdiTransparentBlt(hDC,
			renderer.Left()  + dodgeEffectPos.x,
			renderer.Top() + dodgeEffectPos.y,
			64,
			64,
			hMemDC2,
			effectAnim.iCurrIndex * (int)renderer.Size().X(),
			0,
			64,
			64,
			RGB(255, 0, 255));
	}
}

void CPlayerDodgeState::Release()
{
}

void CPlayerDodgeState::Exit()
{
	static_cast<CPlayer*>(pObj)->bDodgePlaying = false;
}

void CPlayerDodgeState::Enter()
{
	vDodgeDir = static_cast<CPlayer*>(pObj)->vInputDir;
}

void CPlayerDodgeState::Update_AnimFrame()
{
	CState::Update_AnimFrame();

	if (animation.iCurrIndex == 5)
	{
		effectAnim.dwLastPlayTime = GetTickCount();
		effectAnim.iCurrIndex = 0;
		bEffectPlay = true;
	}

	if (bEffectPlay)
	{
		if (effectAnim.dwLastPlayTime + effectAnim.vTransitTime[effectAnim.iCurrIndex] < GetTickCount())
		{
			effectAnim.iCurrIndex++;
			effectAnim.dwLastPlayTime = GetTickCount();

			if (effectAnim.iCurrIndex >= effectAnim.iEndIndex)
			{
				bEffectPlay = false;
			}
		}
	}
}

void CPlayerDodgeState::Stop_Animation()
{
}

void CPlayerDodgeState::On_End_Animation()
{
	pStateMachine->Change_State(CPlayer::PS_WALK);
}

int CPlayerDodgeState::Dir_AnimRow(Direction eDir)
{
	eDir = static_cast<CPlayer*>(pObj)->eDir;
	switch (eDir)
	{
	case D_UP: 
		dodgeEffectPos = { -10, 30 };
		return 0;
	case D_UL: 
		dodgeEffectPos = { 20, 30 };
		return 1;
	case D_UR: 
		dodgeEffectPos = { -40, 30 };
		return 2;
	case D_DOWN: 
		dodgeEffectPos = { -10, -30 }; 
		return 3;
	case D_LEFT: case D_DL:
		dodgeEffectPos = { 20, 0 };
		return 4;
	case D_RIGHT: case D_DR: 
		dodgeEffectPos = { -60, 0 };
		return 5;
	}
	return 0;
}
