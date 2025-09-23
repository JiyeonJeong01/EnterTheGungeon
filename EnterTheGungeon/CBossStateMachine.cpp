#pragma region INCLUDE
#include "pch.h"
#include "CBossStateMachine.h"
#include "CBoss.h"
#include "CBossIdleState.h"
#include "CBossWalkState.h"
#include "CBossDodgeState.h"
#include "CBossCheeseAttackState.h"
#include "CBossSummonAttackState.h"
#include "CBossKunaiAttackState.h"
#include "CObject.h"
#include "CRelease.h"
#pragma endregion

CBossStateMachine::CBossStateMachine(CObject* pObj) : CStateMachine(pObj)
{
    idle = nullptr;
    walk = nullptr;
    dodge = nullptr;
    cheeseAttack = nullptr;
    summonAttack = nullptr;
    kunaiAttack = nullptr;
    curState = prevState = CBoss::BS_END;
}

CBossStateMachine::~CBossStateMachine()
{
    Release();
}

void CBossStateMachine::Initialize()
{
    idle = new CBossIdleState(pObj, this);
    walk = new CBossWalkState(pObj, this);
    dodge = new CBossDodgeState(pObj, this);
    cheeseAttack = new CBossCheeseAttackState(pObj, this);
    summonAttack = new CBossSummonAttackState(pObj, this);
    //kunaiAttack = new CBossKunaiAttackState(pObj, this);

    mBossStates.insert({ CBoss::BS_IDLE,         idle });
    mBossStates.insert({ CBoss::BS_WALK,         walk });
    mBossStates.insert({ CBoss::BS_DODGE,        dodge });
    mBossStates.insert({ CBoss::BS_CHEESEATTACK, cheeseAttack });
    mBossStates.insert({ CBoss::BS_SUMMONATTACK, summonAttack });
    //mBossStates.insert({ CBoss::BS_KUNAIATTACK,  kunaiAttack });

    idle->Initialize();
    walk->Initialize();
    dodge->Initialize();
    cheeseAttack->Initialize();
    summonAttack->Initialize();
    //kunaiAttack->Initialize();

     dynamic_cast<CBoss*>(pObj)->Set_CurrentState(CBoss::BS_IDLE, idle);
     pCurrentState = idle;
     curState = CBoss::BS_IDLE;
     
    //dynamic_cast<CBoss*>(pObj)->Set_CurrentState(CBoss::BS_SUMMONATTACK, summonAttack);
    //pCurrentState = summonAttack;
    //pCurrentState->Enter();
    //curState = CBoss::BS_SUMMONATTACK;
    
}

void CBossStateMachine::Change_State(int iStateKey)
{
    if (iStateKey == pCurrentState->Get_StateID())
    {
        return;
    }

    pCurrentState->Exit();
    pPrevState = pCurrentState;
    prevState = curState;

    pCurrentState = mBossStates[(CBoss::BossState)iStateKey];
    curState = (CBoss::BossState)iStateKey;
    static_cast<CBoss*>(pObj)->Set_CurrentState((CBoss::BossState)iStateKey, pCurrentState);
    pCurrentState->Enter();
}

void CBossStateMachine::Release()
{
    for_each(mBossStates.begin(), mBossStates.end(), [&](auto& pair)->void {
        CRelease<CState*>::Release(pair.second);
        });

    mBossStates.clear();
}
