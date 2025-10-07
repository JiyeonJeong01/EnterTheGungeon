#include "pch.h"
#include "CBossDodgeState.h"
#include "CBossWalkState.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CBossStateMachine.h"
void CBossDodgeState::Initialize()
{
	CBossState::Initialize();
	eState = CBoss::BS_DODGE;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_Dodge.bmp", L"Boss_DODGE");
	animation.Initialize(0, 8, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 90);

	effectAnim.Initialize(0, 5, 0);
	fill(effectAnim.vTransitTime.begin(), effectAnim.vTransitTime.end(), 40);
	bEffectPlay = false;

	bForceDodge = true;
}

void CBossDodgeState::Update()
{
	CBossState::Update();
	Update_AnimFrame();

	if (!bDodgePlaying)
	{
		bDodgePlaying = true;
		static_cast<CBoss*>(pObj)->bDodgePlaying = bDodgePlaying;
	}

	Dodge();
}

void CBossDodgeState::Late_Update()
{
}

void CBossDodgeState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_DODGE");
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
			renderer.Left() + dodgeEffectPos.x,
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

void CBossDodgeState::Release()
{
}

void CBossDodgeState::Exit()
{
	iCurDodge = 0;
	bForceDodge = false;
}

void CBossDodgeState::Enter()
{
	iCurDodge = 0;
}

void CBossDodgeState::Update_AnimFrame()
{
	CState::Update_AnimFrame();

	if (animation.iCurrIndex == 5)
	{
		MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"Player_DODGE.wav", 1.f);
		// MANAGER(CSoundManager*, M_SOUND)->PlaySoundW(L"Player_DODGE.wav", SOUND_EFFECT, 1.f);
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

void CBossDodgeState::Stop_Animation()
{
}

void CBossDodgeState::On_End_Animation()
{
	iCurDodge++;
	if (true /*!bForceDodge*/)
	{
		pStateMachine->Change_State(CBoss::BS_IDLE);
	}
}

int CBossDodgeState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: 
		dodgeEffectPos = { -10, -30 };
		return 3;
	case D_UL: 
		dodgeEffectPos = { -60, 0 };
		return 5;
	case D_UR: 
		dodgeEffectPos = { 20, 0 };
		return 4;
	case D_DOWN: 
		dodgeEffectPos = { -10, 30 };
		return 0;
	case D_LEFT: case D_DL: 
		dodgeEffectPos = { -40, 30 };
		return 2;
	case D_RIGHT: case D_DR: 
		dodgeEffectPos = { 20, 30 };
		return 1;
	}
	return 0;
}

void CBossDodgeState::Dodge()
{
	if (static_cast<CBossStateMachine*>(pStateMachine)->Get_PrevState() == CBoss::BS_IDLE || 
		static_cast<CBossStateMachine*>(pStateMachine)->Get_PrevState() == CBoss::BS_WALK)
	{
		pObj->Get_Transform()->Direction(move(vDirToPlayer * -1.8f));
	}
}
