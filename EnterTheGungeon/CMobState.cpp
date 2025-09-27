#include "pch.h"
#include "CMobState.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CTransform.h"

CMobState::CMobState(CObject* pObj, CStateMachine* pStateMachine) : CState(pObj, pStateMachine)
{
	pPlayer = nullptr;
	eState = CMob::MobEnd;

}

void CMobState::Initialize()
{
	if (pPlayer == nullptr)
	{
		pPlayer = static_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
		fMinDistToPlayer = 70.f;
		fMaxStopDist = 150.f;
		fMinStopDist = 100.f;

		bShouldAttack = false;

		fSpeed = static_cast<CMob*>(pObj)->Get_Speed();
	}
}

void CMobState::Update()
{
	iHP = static_cast<CMob*>(pObj)->Get_HP();

	Detect_Player();
	Check_ShouldAtack();

}

void CMobState::Detect_Player()
{
	Vector2 vPlayerPos = pPlayer->Get_Transform()->Position();
	Vector2 vBossPos = pObj->Get_Transform()->Position();
	Vector2 vDiff = vPlayerPos - vBossPos;

	Compute_Dir(vDiff);
	vDirToPlayer = vDiff.Get_Normalized();
	fCurDistToPlayer = sqrtf(vDiff.X() * vDiff.X() + vDiff.Y() * vDiff.Y());
	bInProperDist = (fCurDistToPlayer >= fMinDistToPlayer && fCurDistToPlayer);
}

void CMobState::Compute_Dir(Vector2 vDiff)
{
	float fAlpha = 150.f;
	if (vDiff.X() > 0)
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

void CMobState::Check_ShouldAtack()
{
	if (dwLastAttackTime + fAttackTimeRange * 1000 < GetTickCount())
	{
		bShouldAttack = true;
	}
	else
	{
		bShouldAttack = false;
	}
}