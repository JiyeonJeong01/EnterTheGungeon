#pragma once
#include "CMob.h"
class CMob03StateMachine;
class CMob03 : public CMob
{
public:
	CMob03();
	~CMob03() override;

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
	CMob03StateMachine* pStateMachine;
	CState* pCurrentState;

private:
	float fAttackTimeRange;

};