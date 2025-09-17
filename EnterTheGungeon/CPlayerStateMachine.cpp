#pragma region INCLUDE
#include "pch.h"
#include "CPlayerStateMachine.h"
#include "CPlayer.h"
#include "CPlayerIdleState.h"
#include "CPlayerWalkState.h"
#include "CPlayerDodgeState.h"
#include "CObject.h"
#include "CRelease.h"
#pragma endregion


CPlayerStateMachine::CPlayerStateMachine(CObject* pObj) : CStateMachine(pObj)
{
}

CPlayerStateMachine::~CPlayerStateMachine()
{
	Release();
}

void CPlayerStateMachine::Initialize()
{
	idle = new CPlayerIdleState(pObj, this);
	walk = new CPlayerWalkState(pObj, this);
	dodge = new CPlayerDodgeState(pObj, this);

	mPlayerStates.insert({ CPlayer::PS_IDLE, idle });
	mPlayerStates.insert({ CPlayer::PS_WALK, walk });
	mPlayerStates.insert({ CPlayer::PS_DODGE, dodge });

	idle->Initialize();
	walk->Initialize();
	dodge->Initialize();
	dynamic_cast<CPlayer*>(pObj)->Set_CurrentState(CPlayer::PS_IDLE, idle);
	pCurrentState = idle;
}

void CPlayerStateMachine::Change_State(int iStateKey)
{
	// CStateMachine::Change_State(iStateKey);
	if (iStateKey == pCurrentState->Get_StateID())
	{
		return;
	}

	pCurrentState->Exit();
	pPrevState = pCurrentState;

	pCurrentState = mPlayerStates[(CPlayer::PlayerState)iStateKey];
	static_cast<CPlayer*>(pObj)->Set_CurrentState((CPlayer::PlayerState)iStateKey, pCurrentState);
	pCurrentState->Enter();
}

void CPlayerStateMachine::Release()
{
	for_each(mPlayerStates.begin(), mPlayerStates.end(), [&](auto& pair)->void {
		CRelease<CState*>::Release(pair.second);
		});
	mPlayerStates.clear();
}
