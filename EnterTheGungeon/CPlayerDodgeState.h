#pragma once
#include "CPlayerState.h"
class CPlayerDodgeState :  public CPlayerState
{
public:
	CPlayerDodgeState(CObject* pObj, CStateMachine* pStateMachine)
		: CPlayerState(pObj, pStateMachine) {
	}
	~CPlayerDodgeState() override {};
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

public :
	Vector2 vDodgeDir;
};
