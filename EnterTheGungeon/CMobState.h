#pragma once
#include "CState.h"
#include "CMob.h"
class CObject;
class CStateMachine;
class CPlayer;

class CMobState :  public CState
{
public:
	CMobState(CObject* pObj, CStateMachine* pStateMachine);
	~CMobState() override {};
public:
	void Initialize() override;
	void Update() override;

public:
	void Detect_Player();
	void Check_ShouldAtack();

	virtual int Dir_AnimRow(Direction eDir) { return 0; };
	void Compute_Dir(Vector2 vDiff);

public:
	CMob::MobState Get_MobState() { return eState;  }

	void Set_Player(CPlayer* pPlayer) { this->pPlayer = pPlayer; }

public:
	int iHP;
	Direction eDir; // need to animRow

	bool bShouldAttack;
	float fAttackTimeRange;
	DWORD dwLastAttackTime;

	float fCurStateMaxTime;
	DWORD dwCurrentStateElapsedTime;

	float fMinDistToPlayer;
	float fMinStopDist;
	float fMaxStopDist;
	float fCurDistToPlayer;
	bool bInProperDist;
	Vector2 vDirToPlayer;

	float fSpeed;

protected :
	CMob::MobState eState;
	CPlayer* pPlayer;
};
