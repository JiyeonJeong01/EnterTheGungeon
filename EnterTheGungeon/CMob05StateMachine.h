#pragma once
#include "CMobStateMachine.h"
#include "CMob.h"

class CMob05IdleState;
class CMob05WalkState;
class CMob05KnockbackState;
class CMob05DeadState;
class CMobState;

class CMob05StateMachine : public CMobStateMachine
{
public:
	CMob05StateMachine(CObject* pObj);
	~CMob05StateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private:
	map<CMob::MobState, CMobState*> mMobStates;
	CMob05IdleState* idle;
	CMob05WalkState* walk;
	CMob05KnockbackState* knockback;
	CMob05DeadState* dead;
private:
	CMob::MobState curState;
	CMob::MobState prevState;
};

