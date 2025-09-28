#pragma once
#include "CMobState.h"

class CMobOwnerDeadState : public CMobState
{
public:
	CMobOwnerDeadState(CObject* pObj, CStateMachine* pStateMachine);
	~CMobOwnerDeadState() override {};

public:
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;

public:
	void Exit() override;
	void Enter() override;
	void Update_AnimFrame() override;
	void On_End_Animation() override;

private:
	int Dir_AnimRow(Direction eDir) override;

	bool bCanMoveAnim;
	int iDeadDir;;
};

