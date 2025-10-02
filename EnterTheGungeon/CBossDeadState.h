#pragma once
#include "CBossState.h"
class CBossDeadState :   public CBossState
{
public:
	CBossDeadState(CObject* pObj, CStateMachine* pStateMachine)
		: CBossState(pObj, pStateMachine) {
	}
	~CBossDeadState() override {};
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


	DWORD dwDeadElapsedTime;
	bool bCreated;
	bool bDontMove;
};

