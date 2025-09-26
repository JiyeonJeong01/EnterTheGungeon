#pragma once
#include "CMob.h"
class CMobState;
class CMob01StateMachine;
class CMob01WalkState;
class CMob01IdleState;
class CMob01DeadState;

class CMob01 : public CMob
{
public:
	CMob01();
	~CMob01() override;

public: // Core
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

	void Update_Transform() override;

public :
	void Set_CurrentState(MobState eState, CState* pState) { eCurrentState = eState; pCurrentState = pState; }

public :
	float Get_AttackTimeRange() { return fAttackTimeRange;  }
private :
	CMob01StateMachine* pStateMachine;
	CState* pCurrentState;

private :
	float fAttackTimeRange;
};

