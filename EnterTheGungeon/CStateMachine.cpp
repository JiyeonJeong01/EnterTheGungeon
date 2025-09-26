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
	// 여기서 에러가 나는데 아무래도 CMob은 얘를 호출해서 생기는 문제인 거 같다 각자의 상태 머신을 해야 한다 ... 
	if (iStateKey == pCurrentState->Get_StateID())
	{
		return;
	}

	pCurrentState->Exit();
	pPrevState = pCurrentState;

    pCurrentState =	vStates[iStateKey];
	pCurrentState->Enter();
}