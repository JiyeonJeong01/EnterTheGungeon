#include "pch.h"
#include "CState.h"
#include "CStateMachine.h"
#include "CObject.h"

CState::CState(CObject* _pObj, CStateMachine* pMachine)
	: pObj(_pObj), pStateMachine(pMachine)
{
	bStarted = false;
	bInProgress = false;
	bExited = false;
	bCanTransit = false;
}

CState::~CState()
{
	Release();
}

void CState::Enter()
{
	bStarted = true;
	bInProgress = true;
	bExited = false;

	animation.iCurrIndex = 0;
	animation.dwLastPlayTime = GetTickCount();
}

void CState::Exit()
{
	bStarted = false;
	bInProgress = false;
	bExited = true;
}

void CState::Move_AnimFrame()
{
	if (animation.dwLastPlayTime + animation.vTransitTime[animation.iCurrIndex] < GetTickCount())
	{
		animation.iCurrIndex++;
		animation.dwLastPlayTime = GetTickCount();

		if (animation.iCurrIndex >= animation.iEndIndex)
		{
			animation.iCurrIndex = 0;
			On_End_Animation();
		}
	}
}

void CState::Stop_Animation()
{

}

void CState::On_End_Animation() { }
