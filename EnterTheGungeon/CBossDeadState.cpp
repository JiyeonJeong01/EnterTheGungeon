#include "pch.h"
#include "CBossDeadState.h"
#include "CBmpManager.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CBossStateMachine.h"
void CBossDeadState::Initialize()
{
	CBossState::Initialize();
	eState = CBoss::BS_DEAD;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_DEAD.bmp", L"Boss_DEAD");
	animation.Initialize(0, 7, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 120);
}

void CBossDeadState::Update()
{
	Update_AnimFrame();
}

void CBossDeadState::Late_Update()
{
}

void CBossDeadState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_DEAD");

	CRenderer renderer = *(pObj->Get_Renderer());

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)renderer.Size().X(),
		0 * (int)renderer.Size().Y(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		RGB(255, 0, 255));
}

void CBossDeadState::Release()
{
}

void CBossDeadState::Exit()
{
}

void CBossDeadState::Enter()
{
	animation.iCurrIndex = 0;
}

void CBossDeadState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
	if (animation.iCurrIndex >= animation.iEndIndex)
	{
		animation.iCurrIndex = 0;
	}
}

void CBossDeadState::Stop_Animation()
{
}

void CBossDeadState::On_End_Animation()
{
}

int CBossDeadState::Dir_AnimRow(Direction eDir)
{
	return 0;
}
