#pragma once
#include "CAnimation.h"

class CStateMachine;
class CObject;

class CState
{
public:
	CState(CObject* _pObj, CStateMachine* pMachine);
	virtual ~CState();

public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Late_Update() {};
	virtual void Render(HDC hDC) {};
	virtual void Release() {};

public:
	virtual void Exit();
	virtual void Enter();

	virtual void Update_AnimFrame();
	virtual void Stop_Animation();
	virtual void On_End_Animation();

public:
	void Set_Object(CObject* pObj) { this->pObj = pObj; }

	bool Get_Started() const { return bStarted; }
	bool Get_InProgress() const { return bInProgress; }
	bool Get_Exited() const { return bExited; }
	bool Get_CanTransit() const { return bCanTransit; }

	int Get_StateID() const { return iState; }

protected:
	CObject* pObj;
	CStateMachine* pStateMachine;
	CAnimation animation;

protected:
	bool bStarted;
	bool bInProgress;
	bool bExited; // This state has completed
	bool bCanTransit; // This state can transition to another sate anytime

	int iState;
};

