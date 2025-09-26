#pragma once
#include "CMobStateMachine.h"
#include "CMob.h"

class CMob03IdleState;
class CMob03WalkState;
class CMob03KnockbackState;
class CMob03DeadState;
class CMobState;

class CMob03StateMachine : public CMobStateMachine
{
public:
	CMob03StateMachine(CObject* pObj);
	~CMob03StateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private:
	map<CMob::MobState, CMobState*> mMobStates;
	CMob03IdleState* idle;
	CMob03WalkState* walk;
	CMob03KnockbackState* knockback;
	CMob03DeadState* dead;
private:
	CMob::MobState curState;
	CMob::MobState prevState;
};

