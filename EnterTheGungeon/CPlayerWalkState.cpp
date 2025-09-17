#include "pch.h"
#include "CPlayerWalkState.h"
#include "CTransform.h"
#include "CManager.h"
#include "CBmpManager.h"
#include "CInputManager.h"
#include "CRenderer.h"
#include "CStateMachine.h"
#include "CTransform.h"

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
	Move_AnimFrame();

	if (vInputDir.X() == 0.f && vInputDir.Y() == 0.f)
	{
		pStateMachine->Change_State((CPlayer::PS_IDLE));
		return;
	}
	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_LBUTTON))
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
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Player_WALK");

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

void CPlayerWalkState::Release()
{
}

void CPlayerWalkState::Exit()
{
}

void CPlayerWalkState::Enter()
{
}

void CPlayerWalkState::Move_AnimFrame()
{
	CState::Move_AnimFrame();
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
