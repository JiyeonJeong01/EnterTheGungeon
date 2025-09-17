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



	Move_AnimFrame();
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
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Player_IDLE");

	CRenderer renderer = *(pObj->Get_Renderer());
	CTransform transform = *(pObj->Get_Transform());

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)transform.Size().X(),
		(int)transform.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)transform.Size().X(),
		Dir_AnimRow(eDir) * (int)transform.Size().Y(),
		(int)transform.Size().X(),
		(int)transform.Size().Y(),
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

void CPlayerIdleState::Move_AnimFrame()
{
	CState::Move_AnimFrame();
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
	case D_UP: return 0;
	case D_UL: return 1;
	case D_UR: return 2;
	case D_DOWN: return 3;
	case D_LEFT: case D_DL: return 4;
	case D_RIGHT: case D_DR: return 5;
	}
	return 0;
}
