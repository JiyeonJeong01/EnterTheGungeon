#pragma once
#include "CMob.h"

class CMobState;
class CMob02StateMachine;
class CMob02WalkState;
class CMob02IdleState;
class CMob02DeadState;

class CMob02 :  public CMob
{
public:
	CMob02();
	~CMob02() override;

public: // Core
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

	void Update_Transform() override;

public:
	void Set_CurrentState(MobState eState, CState* pState) { eCurrentState = eState; pCurrentState = pState; }

public:
	float Get_AttackTimeRange() { return fAttackTimeRange; }
private:
	CMob02StateMachine* pStateMachine;
	CState* pCurrentState;

private:
	float fAttackTimeRange;

};