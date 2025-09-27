#pragma once
#include "CMob.h"
class CMobState;
class CMob05StateMachine;

class CMob05 : public CMob
{
public:
	CMob05();
	~CMob05() override;

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
	CMob05StateMachine* pStateMachine;
	CState* pCurrentState;

private:
	float fAttackTimeRange;

};

