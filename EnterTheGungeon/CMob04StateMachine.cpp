#include "pch.h"
#include "CMob04.h"

#include "CMob04StateMachine.h"
#include "CMob04IdleState.h"
#include "CMob04WalkState.h"
#include "CMob04KnockbackState.h"
#include "CMob04DeadState.h"

#include "CRelease.h"
CMob04StateMachine::CMob04StateMachine(CObject* pObj) : CMobStateMachine(pObj)
{
	idle = nullptr;
}

CMob04StateMachine::~CMob04StateMachine()
{
	Release();
}

void CMob04StateMachine::Initialize()
{
	idle = new CMob04IdleState(pObj, this);
	walk = new CMob04WalkState(pObj, this);
	knockback = new CMob04KnockbackState(pObj, this);
	dead = new CMob04DeadState(pObj, this);

	mMobStates.insert({ CMob::Idle, idle });
	mMobStates.insert({ CMob::Walk, walk });
	mMobStates.insert({ CMob::Damaged, knockback });
	mMobStates.insert({ CMob::Dead, dead });

	idle->Initialize();
	walk->Initialize();
	knockback->Initialize();
	dead->Initialize();

	dynamic_cast<CMob04*>(pObj)->Set_CurrentState(CMob::Idle, idle);
	pCurrentState = idle;
	curState = CMob::Idle;
}

void CMob04StateMachine::Change_State(int iStateKey)
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
	static_cast<CMob04*>(pObj)->Set_CurrentState((CMob::MobState)iStateKey, pCurrentState);

}

void CMob04StateMachine::Release()
{
	CRelease<CMob04IdleState*>::Release(idle);
	CRelease<CMob04WalkState*>::Release(walk);
	CRelease<CMob04KnockbackState*>::Release(knockback);
	CRelease<CMob04DeadState*>::Release(dead);
	mMobStates.clear();
}
