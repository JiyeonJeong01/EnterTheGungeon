#include "pch.h"
#include "CStateMachine.h"
#include "CState.h"

CStateMachine::CStateMachine(CObject* _pObj)
{
	this->pObj = _pObj;

	pPrevState = nullptr;
	pCurrentState = nullptr;
	pNextState = nullptr;
}

CStateMachine::~CStateMachine()
{
}

void CStateMachine::Initialize()
{
}

void CStateMachine::Change_State(int iStateKey)
{
	if (iStateKey == pCurrentState->Get_StateID())
	{
		return;
	}

	pCurrentState->Exit();
	pPrevState = pCurrentState;

    pCurrentState =	vStates[iStateKey];
	pCurrentState->Enter();
}