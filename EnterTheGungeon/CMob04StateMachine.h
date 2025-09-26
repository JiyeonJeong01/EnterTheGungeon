#pragma once
#include "CMobStateMachine.h"
#include "CMob.h"

class CMob04IdleState;
class CMob04WalkState;
class CMob04KnockbackState;
class CMob04DeadState;
class CMobState;

class CMob04StateMachine : public CMobStateMachine
{
public:
	CMob04StateMachine(CObject* pObj);
	~CMob04StateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private:
	map<CMob::MobState, CMobState*> mMobStates;
	CMob04IdleState* idle;
	CMob04WalkState* walk;
	CMob04KnockbackState* knockback;
	CMob04DeadState* dead;
private:
	CMob::MobState curState;
	CMob::MobState prevState;
};

