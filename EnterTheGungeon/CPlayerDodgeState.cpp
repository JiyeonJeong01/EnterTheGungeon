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
	animation.Initialize(0, 8, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 40.f);
	vDodgeDir = { 0.f, 0.f };
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
