#pragma once
#include "CStateMachine.h"
#include "CBoss.h"
class CBossIdleState;
class CBossWalkState;
class CBossDodgeState;
class CBossCheeseAttackState;
class CBossSummonAttackState;
class CBossKunaiAttackState;

class CBossStateMachine : public CStateMachine
{
public:
	CBossStateMachine(CObject* pObj);
	~CBossStateMachine() override;

public:
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

public :
	CBoss::BossState Get_PrevState() { return prevState; }

private:
	map<CBoss::BossState, CState*> mBossStates;
	CBossIdleState* idle;
	CBossWalkState* walk;
	CBossDodgeState* dodge;
	CBossCheeseAttackState* cheeseAttack;
	CBossSummonAttackState* summonAttack;
	CBossKunaiAttackState* kunaiAttack;

private :
	CBoss::BossState curState;
	CBoss::BossState prevState;
};