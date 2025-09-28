#pragma once
#include "CMobState.h"
class CObject;
class CStateMachine;

class CMobOwnerWalkState : public CMobState
{
public:
	CMobOwnerWalkState(CObject* pObj, CStateMachine* pStateMachine);
	~CMobOwnerWalkState() override {};

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
	int Get_WeaponPos();
	int Get_WeaponDir();

public:
	void Do_Attack();
	void Do_KnockBack();

private:
	void Attack01();
	void Attack02();
	void Attack03();

private:
	void Pre_Calculate();

private:
	bool bCanAttack;
	bool bAttackTimer;
	int iCurrentAttackCount;

	int iAnimRow;

private:
	list<Vector2> vNormalizedDir01;
};

