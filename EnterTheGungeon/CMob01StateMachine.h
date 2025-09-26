#pragma once
#include "CMobStateMachine.h"
#include "CMob.h"

class CMob01IdleState;
class CMob01WalkState;
class CMob01KnockbackState;
class CMob01DeadState;
class CMobState;

class CMob01StateMachine :     public CMobStateMachine
{
public:
	CMob01StateMachine(CObject* pObj);
	~CMob01StateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private :
	map<CMob::MobState, CMobState*> mMobStates;
	CMob01IdleState* idle;
	CMob01WalkState* walk;
	CMob01KnockbackState* knockback;
	CMob01DeadState* dead;
private:
	CMob::MobState curState;
	CMob::MobState prevState;
};

