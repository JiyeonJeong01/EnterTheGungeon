#pragma once
#include "CMobStateMachine.h"
#include "CMob.h"

class CMobOwnerIdleState;
class CMobOwnerWalkState;
class CMobOwnerKnockbackState;
class CMobOwnerDeadState;
class CMobState;

class CMobOwnerStateMachine : public CMobStateMachine
{
public:
	CMobOwnerStateMachine(CObject* pObj);
	~CMobOwnerStateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private:
	map<CMob::MobState, CMobState*> mMobStates;
	CMobOwnerIdleState* idle;
	CMobOwnerWalkState* walk;
	CMobOwnerKnockbackState* knockback;
	CMobOwnerDeadState* dead;
private:
	CMob::MobState curState;
	CMob::MobState prevState;
};

