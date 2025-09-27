#include "pch.h"
#include "CMob01.h"

#include "CMob01StateMachine.h"
#include "CMob01IdleState.h"
#include "CMob01WalkState.h"
#include "CMob01KnockbackState.h"
#include "CMob01DeadState.h"

#include "CRelease.h"
CMob01StateMachine::CMob01StateMachine(CObject* pObj) : CMobStateMachine(pObj)
{
	idle = nullptr;
}

CMob01StateMachine::~CMob01StateMachine()
{
	Release();
}

void CMob01StateMachine::Initialize()
{
	idle = new CMob01IdleState(pObj, this);
	walk = new CMob01WalkState(pObj, this);
	knockback = new CMob01KnockbackState(pObj, this);
	dead = new CMob01DeadState(pObj, this);

	mMobStates.insert({ CMob::Idle, idle });
	mMobStates.insert({ CMob::Walk, walk });
	mMobStates.insert({ CMob::Damaged, knockback });
	mMobStates.insert({ CMob::Dead, dead });

	idle->Initialize();
	walk->Initialize();
	knockback->Initialize();
	dead->Initialize();

	dynamic_cast<CMob01*>(pObj)->Set_CurrentState(CMob::Idle, idle);
	pCurrentState = idle;
	curState = CMob::Idle;
}

void CMob01StateMachine::Change_State(int iStateKey)
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
	static_cast<CMob01*>(pObj)->Set_CurrentState((CMob::MobState)iStateKey, pCurrentState); 

}

void CMob01StateMachine::Release()
{
	CRelease<CMob01IdleState*>::Release(idle);
	CRelease<CMob01WalkState*>::Release(walk);
	CRelease<CMob01KnockbackState*>::Release(knockback);
	CRelease<CMob01DeadState*>::Release(dead);


	mMobStates.clear();
}
