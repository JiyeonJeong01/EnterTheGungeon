#pragma once
#include "CBossState.h"
class CBossWalkState :  public CBossState
{
public:
	CBossWalkState(CObject* pObj, CStateMachine* pStateMachine)
		: CBossState(pObj, pStateMachine) {
	}
	~CBossWalkState() override {};
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

private :
	void Walk();

private :
	Vector2 walkDir;
	bool bWalkFinish;
};

