#include "pch.h"
#include "CMob03.h"

#include "CMob03StateMachine.h"
#include "CMob03IdleState.h"
#include "CMob03WalkState.h"
#include "CMob03KnockbackState.h"
#include "CMob03DeadState.h"

#include "CRelease.h"
CMob03StateMachine::CMob03StateMachine(CObject* pObj) : CMobStateMachine(pObj)
{
	idle = nullptr;
}

CMob03StateMachine::~CMob03StateMachine()
{
	Release();
}

void CMob03StateMachine::Initialize()
{
	idle = new CMob03IdleState(pObj, this);
	walk = new CMob03WalkState(pObj, this);
	knockback = new CMob03KnockbackState(pObj, this);
	dead = new CMob03DeadState(pObj, this);

	mMobStates.insert({ CMob::Idle, idle });
	mMobStates.insert({ CMob::Walk, walk });
	mMobStates.insert({ CMob::Damaged, knockback });
	mMobStates.insert({ CMob::Dead, dead });

	idle->Initialize();
	walk->Initialize();
	knockback->Initialize();
	dead->Initialize();

	dynamic_cast<CMob03*>(pObj)->Set_CurrentState(CMob::Idle, idle);
	pCurrentState = idle;
	curState = CMob::Idle;
}

void CMob03StateMachine::Change_State(int iStateKey)
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
	static_cast<CMob03*>(pObj)->Set_CurrentState((CMob::MobState)iStateKey, pCurrentState);

}

void CMob03StateMachine::Release()
{
	CRelease<CMob03IdleState*>::Release(idle);
}
