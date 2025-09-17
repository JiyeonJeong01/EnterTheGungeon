#pragma once
#include "CStateMachine.h"
#include "CPlayer.h"
class CPlayerIdleState;
class CPlayerWalkState;
class CPlayerDodgeState;
class CPlayerStateMachine : public CStateMachine
{
public :
	CPlayerStateMachine(CObject* pObj);
	~ CPlayerStateMachine() override;

public :
	void Initialize() override;
	void Change_State(int iStateKey) override;
	void Release() override;

private :
	map<CPlayer::PlayerState, CState*> mPlayerStates;
	CPlayerIdleState* idle;
	CPlayerWalkState* walk;
	CPlayerDodgeState* dodge;
};

