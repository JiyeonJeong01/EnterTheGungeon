#pragma once
#include "CMobStateMachine.h"
#include "CMob.h"

class CMob02IdleState;
class CMob02WalkState;
class CMob02KnockbackState;
class CMob02DeadState;
class CMobState;

class CMob02StateMachine : public CMobStateMachine
{
public:
	CMob02StateMachine(CObject* pObj);
	~CMob02StateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private:
	map<CMob::MobState, CMobState*> mMobStates;
	CMob02IdleState* idle;
	CMob02WalkState* walk;
	CMob02KnockbackState* knockback;
	CMob02DeadState* dead;
private:
	CMob::MobState curState;
	CMob::MobState prevState;
};

