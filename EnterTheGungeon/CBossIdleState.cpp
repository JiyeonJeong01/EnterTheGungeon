#include "pch.h"
#include "CBossIdleState.h"
#include "CBmpManager.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CBossStateMachine.h"

void CBossIdleState::Initialize()
{
	CBossState::Initialize();
	eState = CBoss::BS_IDLE;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_IDLE.bmp", L"Boss_IDLE");
	animation.Initialize(0, 3, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);

	fCurStateMaxTime = 2.2f;
}

void CBossIdleState::Update()
{
	CBossState::Update();
	Update_AnimFrame();
}

void CBossIdleState::Late_Update()
{
	CBossState::Detect_Player();

	if (bShouldAttack)
	{
		static_cast<CBoss*>(pObj)->Check_ShouldAtack();
		if (static_cast<CBoss*>(pObj)->bShouldAttack)
		{
			if (static_cast<CBossStateMachine*>(pStateMachine)->Get_PrevAttack() != CBoss::BS_CHEESEATTACK)
			{
				pStateMachine->Change_State(CBoss::BS_CHEESEATTACK);
			}
			else if (static_cast<CBossStateMachine*>(pStateMachine)->Get_PrevAttack() != CBoss::BS_SUMMONATTACK)
			{
				pStateMachine->Change_State(CBoss::BS_SUMMONATTACK);
			}
		}
	}
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount()
		&& (fCurDistToPlayer <= fMinStopDist || fCurDistToPlayer >= fMaxStopDist))
	{
		pStateMachine->Change_State(CBoss::BS_WALK);
	}
	if (fCurDistToPlayer <= fMinDistToPlayer)
	{
		pStateMachine->Change_State(CBoss::BS_DODGE);
	}

}

void CBossIdleState::Render(HDC hDC)
{

	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_IDLE");

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

void CBossIdleState::Release()
{
}

void CBossIdleState::Exit()
{
}

void CBossIdleState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	if (static_cast<CBossStateMachine*>(pStateMachine)->Get_PrevState() == CBoss::BS_WALK)
	{
		dwCurrentStateElapsedTime += fCurStateMaxTime * 500;
	}
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
}

void CBossIdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CBossIdleState::Stop_Animation()
{
}

void CBossIdleState::On_End_Animation()
{
}

int CBossIdleState::Dir_AnimRow(Direction eDir)
{
	return 0;
}
