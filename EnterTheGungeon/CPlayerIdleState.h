#pragma once
#include "CPlayerState.h"

class CPlayerIdleState : public CPlayerState
{
public :
	CPlayerIdleState(CObject* pObj, CStateMachine* pStateMachine)
		: CPlayerState(pObj, pStateMachine) {
	}
	~CPlayerIdleState() override {};
public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Exit() override;
	void Enter() override;

	void Move_AnimFrame() override;
	void Stop_Animation() override;
	void On_End_Animation() override;

	int Dir_AnimRow(Direction eDir) override;
};

