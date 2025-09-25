#pragma once
#include "CState.h"
#include "CBoss.h"
class CObject;
class CStateMachine;
class CPlayer;

class CBossState :    public CState
{
public:
	CBossState(CObject* pObj, CStateMachine* pStateMachine);
	~CBossState() override {};

public:
	void Initialize() override;
	void Update() override;

public:
	virtual int Dir_AnimRow(Direction eDir) { return 0; };
	void Compute_Dir(Vector2 vDiff);
	void Compute_CurrentStateElapsedTime();
	void Detect_Player();

	CBoss::BossState Get_State() { return eState; }
	DWORD Get_CurrentStateElapsedTime() { return dwCurrentStateElapsedTime; }
	void Set_Player(CPlayer* pPlayer) { this->pPlayer = pPlayer; }

protected:
	CBoss::BossState eState;
	CPlayer* pPlayer;

protected:
	int iHP;
	CBoss::BossState prevAttackType;
	float fCurStateMaxTime;
	DWORD dwCurrentStateElapsedTime;

	DWORD dwLastFireTime;
	float fLimitFireTime;

	float fSpeed;
	bool bDodgePlaying;
	Direction eDir; // need to animRow

	float fMaxDistToPlayer;
	float fMinDistToPlayer;
	float fMinStopDist;
	float fMaxStopDist;
	float fCurDistToPlayer;
	bool bInProperDist;
	Vector2 vDirToPlayer;

	bool bShouldAttack;

};

