#pragma once
#include "CMobState.h"
class CMob03DeadState :   public CMobState
{
public:
	CMob03DeadState(CObject* pObj, CStateMachine* pStateMachine);
	~CMob03DeadState() override {};

public:
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;

public:
	void Exit() override;
	void Enter() override;
	void On_End_Animation() override;

private:
	int Dir_AnimRow(Direction eDir) override;

	bool bCanMoveAnim;
	int iDeadDir;;
};

