#include "pch.h"
#include "CBossDeadState.h"
#include "CBmpManager.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CBossStateMachine.h"
#include "CStageManager.h"
void CBossDeadState::Initialize()
{
	CBossState::Initialize();
	eState = CBoss::BS_DEAD;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_DEAD.bmp", L"Boss_DEAD");
	animation.Initialize(0, 7, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 120);
	bCreated = false;
	bDontMove = false;
}

void CBossDeadState::Update()
{
	Update_AnimFrame();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });

	if (!bCreated && (dwDeadElapsedTime + 200 < GetTickCount()))
	{
		bCreated = true;
		MANAGER(CStageManager*, M_STAGE)->On_BossDead();
	}
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
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CBossDeadState::Update_AnimFrame()
{
	if (!bDontMove && animation.dwLastPlayTime + animation.vTransitTime[animation.iCurrIndex] < GetTickCount())
	{
		animation.iCurrIndex++;
		animation.dwLastPlayTime = GetTickCount();
		if (animation.iCurrIndex >= animation.iEndIndex)
		{
			bDontMove = true;
			dwDeadElapsedTime = GetTickCount();
		}
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
