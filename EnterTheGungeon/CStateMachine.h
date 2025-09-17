#pragma once

class CState;
class CObject;

class CStateMachine
{
public:
	CStateMachine(CObject* pObj);
	virtual ~CStateMachine();

public:
	virtual void Initialize();
	virtual void Change_State(int iStateKey);
	virtual void Release() {};

protected:
	CObject* pObj;

protected:
	vector<CState*> vStates;

	CState* pPrevState;
	CState* pCurrentState;
	CState* pNextState;
};

