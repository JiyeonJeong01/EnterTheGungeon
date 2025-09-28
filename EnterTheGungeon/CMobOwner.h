#pragma once
#include "CMob.h"
class CMobOwnerStateMachine;
class CMobOwner : public CMob
{
public:
	CMobOwner();
	~CMobOwner() override;

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
	CMobOwnerStateMachine* pStateMachine;
	CState* pCurrentState;

private:
	float fAttackTimeRange;

};