#pragma once
#include "CStateMachine.h"
class CMobStateMachine :  public CStateMachine
{
public:
	CMobStateMachine(CObject* pObj);
	~CMobStateMachine() override;
};

