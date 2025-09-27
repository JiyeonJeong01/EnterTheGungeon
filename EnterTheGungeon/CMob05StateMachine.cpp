#include "pch.h"
#include "CMob05.h"

#include "CMob05StateMachine.h"
#include "CMob05IdleState.h"
#include "CMob05WalkState.h"
#include "CMob05KnockbackState.h"
#include "CMob05DeadState.h"

#include "CRelease.h"
CMob05StateMachine::CMob05StateMachine(CObject* pObj) : CMobStateMachine(pObj)
{
	idle = nullptr;
}

CMob05StateMachine::~CMob05StateMachine()
{
	Release();
}

void CMob05StateMachine::Initialize()
{
	idle = new CMob05IdleState(pObj, this);
	walk = new CMob05WalkState(pObj, this);
	knockback = new CMob05KnockbackState(pObj, this);
	dead = new CMob05DeadState(pObj, this);

	mMobStates.insert({ CMob::Idle, idle });
	mMobStates.insert({ CMob::Walk, walk });
	mMobStates.insert({ CMob::Damaged, knockback });
	mMobStates.insert({ CMob::Dead, dead });

	idle->Initialize();
	walk->Initialize();
	knockback->Initialize();
	dead->Initialize();

	dynamic_cast<CMob05*>(pObj)->Set_CurrentState(CMob::Idle, idle);
	pCurrentState = idle;
	curState = CMob::Idle;
}

void CMob05StateMachine::Change_State(int iStateKey)
{
	if (iStateKey == pCurrentState->Get_StateID())
	{
		return;
	}

	pCurrentState->Exit();
	pPrevState = pCurrentState;
	prevState = curState;

	pCurrentState = mMobStates[(CMob::MobState)iStateKey];
	curState = (CMob::MobState)iStateKey;
	pCurrentState->Enter();
	static_cast<CMob05*>(pObj)->Set_CurrentState((CMob::MobState)iStateKey, pCurrentState);

}

void CMob05StateMachine::Release()
{
	CRelease<CMob05IdleState*>::Release(idle);
	CRelease<CMob05WalkState*>::Release(walk);
	CRelease<CMob05KnockbackState*>::Release(knockback);
	CRelease<CMob05DeadState*>::Release(dead);

	mMobStates.clear();
}
