#pragma once
#include "CObject.h"

class CBossStateMachine;
class CBossState;
class CState;

class CBoss :  public CObject
{
public:
	enum BossState { BS_IDLE, BS_WALK, BS_HIT, BS_DODGE, BS_DEAD,
		BS_CHEESEATTACK, BS_SUMMONATTACK, BS_KUNAIATTACK, BS_END };
public:
	CBoss();
	virtual ~CBoss();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public: // Components
	void Update_Transform() override;
	void Update_Renderer() override;

public :
	void OnCollision(CObject* pObj, Vector2 vDiff) override;

public:
	void Initialize_BossComponents();
	void Check_ShouldAtack();

public:
	void Set_CurrentState(BossState eState, CState* pState) { eCurrentState = eState; pCurrentState = pState; }
	void Set_LastAttackTime() { dwLastAttackTime = GetTickCount(); }
	void Set_Speed(float fSpeed) { this->fSpeed = fSpeed; }


public:
	DWORD dwLastFireTime;
	float fLimitFireTime;
	float fSpeed;
	bool bDodgePlaying;
	bool bShouldAttack;
	DWORD dwLastAttackTime;

public:
	int iHP;
	int iMaxHP;

private:
	CBossStateMachine* pStateMachine;
	CState* pCurrentState;
	BossState eCurrentState;
private :
	float fAttackTimeRange;
};

