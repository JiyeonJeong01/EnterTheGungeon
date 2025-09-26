#include "pch.h"
#include "CMob02.h"

#include "CMob02StateMachine.h"
#include "CMob02IdleState.h"
#include "CMob02WalkState.h"
#include "CMob02KnockbackState.h"
#include "CMob02DeadState.h"

#include "CRelease.h"
CMob02StateMachine::CMob02StateMachine(CObject* pObj) : CMobStateMachine(pObj)
{
	idle = nullptr;
}

CMob02StateMachine::~CMob02StateMachine()
{
	Release();
}

void CMob02StateMachine::Initialize()
{
	idle = new CMob02IdleState(pObj, this);
	walk = new CMob02WalkState(pObj, this);
	knockback = new CMob02KnockbackState(pObj, this);
	dead = new CMob02DeadState(pObj, this);

	mMobStates.insert({ CMob::Idle, idle });
	mMobStates.insert({ CMob::Walk, walk });
	mMobStates.insert({ CMob::Damaged, knockback });
	mMobStates.insert({ CMob::Dead, dead });

	idle->Initialize();
	walk->Initialize();
	knockback->Initialize();
	dead->Initialize();

	dynamic_cast<CMob02*>(pObj)->Set_CurrentState(CMob::Idle, idle);
	pCurrentState = idle;
	curState = CMob::Idle;
}

void CMob02StateMachine::Change_State(int iStateKey)
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
	static_cast<CMob02*>(pObj)->Set_CurrentState((CMob::MobState)iStateKey, pCurrentState);

}

void CMob02StateMachine::Release()
{
	CRelease<CMob02IdleState*>::Release(idle);
}
