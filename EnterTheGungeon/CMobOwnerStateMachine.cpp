#include "pch.h"
#include "CMobOwner.h"
#include "CMobOwnerStateMachine.h"
#include "CMobOwnerIdleState.h"
#include "CMobOwnerWalkState.h"
#include "CMobOwnerKnockbackState.h"
#include "CMobOwnerDeadState.h"

#include "CRelease.h"
CMobOwnerStateMachine::CMobOwnerStateMachine(CObject* pObj) : CMobStateMachine(pObj)
{
	idle = nullptr;
}

CMobOwnerStateMachine::~CMobOwnerStateMachine()
{
	Release();
}

void CMobOwnerStateMachine::Initialize()
{
	idle = new CMobOwnerIdleState(pObj, this);
	walk = new CMobOwnerWalkState(pObj, this);
	knockback = new CMobOwnerKnockbackState(pObj, this);
	dead = new CMobOwnerDeadState(pObj, this);

	mMobStates.insert({ CMob::Idle, idle });
	mMobStates.insert({ CMob::Walk, walk });
	mMobStates.insert({ CMob::Damaged, knockback });
	mMobStates.insert({ CMob::Dead, dead });

	idle->Initialize();
	walk->Initialize();
	knockback->Initialize();
	dead->Initialize();

	dynamic_cast<CMobOwner*>(pObj)->Set_CurrentState(CMob::Idle, idle);
	pCurrentState = idle;
	pCurrentState->Enter();
	curState = CMob::Idle;
}

void CMobOwnerStateMachine::Change_State(int iStateKey)
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
	static_cast<CMobOwner*>(pObj)->Set_CurrentState((CMob::MobState)iStateKey, pCurrentState);

}

void CMobOwnerStateMachine::Release()
{
	CRelease<CMobOwnerIdleState*>::Release(idle);
	CRelease<CMobOwnerWalkState*>::Release(walk);
	CRelease<CMobOwnerKnockbackState*>::Release(knockback);
	CRelease<CMobOwnerDeadState*>::Release(dead);
	mMobStates.clear();
}
