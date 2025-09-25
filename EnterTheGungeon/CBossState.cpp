#pragma region MyRegion
#include "pch.h"
#include "CBossState.h"

#include "CObjectManager.h"

#include "CPlayer.h"
#include "CTransform.h"
#pragma endregion


CBossState::CBossState(CObject* pObj, CStateMachine* pStateMachine) : CState(pObj, pStateMachine)
{
	pPlayer = nullptr;
	eState = CBoss::BossState::BS_END;
	prevAttackType = CBoss::BossState::BS_END;

	dwCurrentStateElapsedTime = 0;
}


void CBossState::Initialize()
{
	if (pPlayer == nullptr)
	{
		pPlayer = static_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
		fMaxDistToPlayer = 800.f;
		fMinDistToPlayer = 200.f;
		fMaxStopDist = 450.f;
		fMinStopDist = 350.f;

		bShouldAttack = false;

		fSpeed = static_cast<CPlayer*>(pObj)->fSpeed;
	}
}

void CBossState::Update()
{
	iHP = static_cast<CBoss*>(pObj)->iHP;
	bDodgePlaying = static_cast<CBoss*>(pObj)->bDodgePlaying;
	bShouldAttack = static_cast<CBoss*>(pObj)->bShouldAttack;

	Compute_CurrentStateElapsedTime();
	Detect_Player();
}

void CBossState::Compute_Dir(Vector2 vDiff)
{
	float fAlpha = 150.f;
	if (vDiff.X() > 0) // 커서가 플레이어의 오른쪽
	{
		if (vDiff.Y() < 0)
		{
			if (vDiff.X() < fAlpha) eDir = D_UP;
			else 	eDir = D_UR;
		}
		else
		{
			if (vDiff.X() < fAlpha) 	eDir = D_DOWN;
			else 	eDir = D_DR;
		}
	}
	else
	{
		if (vDiff.Y() < 0)
		{
			if (vDiff.X() > -fAlpha)  	eDir = D_UP;
			else  	eDir = D_UL;
		}
		else
		{
			if (vDiff.X() > -fAlpha) 	eDir = D_DOWN;
			else 		eDir = D_DL;
		}
	}
}

void CBossState::Compute_CurrentStateElapsedTime()
{
	dwCurrentStateElapsedTime += GetTickCount();
}

void CBossState::Detect_Player()
{
	printf("detect player\n");
	Vector2 vPlayerPos = pPlayer->Get_Transform()->Position();
	Vector2 vBossPos = pObj->Get_Transform()->Position();
	Vector2 vDiff = vPlayerPos - vBossPos;

	Compute_Dir(vDiff);
	vDirToPlayer = vDiff.Get_Normalized();
	fCurDistToPlayer = sqrtf(vDiff.X() * vDiff.X() + vDiff.Y() * vDiff.Y());
	bInProperDist = (fCurDistToPlayer >= fMinDistToPlayer && fCurDistToPlayer <= fMaxDistToPlayer);
}

