#pragma once
#include "CBossState.h"
class CBossCheeseAttackState :  public CBossState
{
public:
	CBossCheeseAttackState(CObject* pObj, CStateMachine* pStateMachine)
		: CBossState(pObj, pStateMachine) {
	}
	~CBossCheeseAttackState() override {};
public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Exit() override;
	void Enter() override;

	void Update_AnimFrame() override;
	void Stop_Animation() override;
	void On_End_Animation() override;

	int Dir_AnimRow(Direction eDir) override;


private:
	void Attack_CheeseBullet();
	void Attack_CircleBullet();

private :
	float fCurAttackDegree;
	int iPrevFrame;
	float fFireTimeRange;
	float fFireDegreeRange;
	DWORD dwLastFireTime;
	bool bAnimLoop;
	int iAnimLoopCount;

	int iRandAttack;
};

