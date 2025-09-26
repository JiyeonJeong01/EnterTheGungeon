#pragma once
#include "CMob.h"
class CMobState;
class CMob04StateMachine;

class CMob04 :  public CMob
{
public:
	CMob04();
	~CMob04() override;

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
	CMob04StateMachine* pStateMachine;
	CState* pCurrentState;

private:
	float fAttackTimeRange;

};

