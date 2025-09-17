#pragma region INCLUDE
#include "pch.h"
#include "CInputManager.h"

#include "CManager.h"
#include "CRelease.h"

#include "CInputKeyInfo.h"
#pragma endregion

CInputManager::CInputManager()
{

	pCursor = {0, 0};
}

CInputManager::~CInputManager()
{
	Release();
}

void CInputManager::Initialize()
{
	for (vector<int>::iterator it = vKeys.begin(); it != vKeys.end(); it++)
	{
		mKeyInfos.insert({ *it, new CInputKeyInfo() });
	}
}

void CInputManager::Update()
{
	Check_KeyInput();
	Check_CursorPosition();
}

void CInputManager::Release()
{
	for_each(mKeyInfos.begin(), mKeyInfos.end(),
		[](pair<int, CInputKeyInfo*> value) -> void { CRelease<CInputKeyInfo*>::Release(value.second); });
	 mKeyInfos.clear();
}

bool CInputManager::Get_KeyDown(int iKey)
{
	return mKeyInfos[iKey]->Get_KeyDown();
}

bool CInputManager::Get_Key(int iKey)
{
	return mKeyInfos[iKey]->Get_Key();
}

bool CInputManager::Get_KeyUp(int iKey)
{
	return mKeyInfos[iKey]->Get_KeyUp();
}

void CInputManager::Check_KeyInput()
{
	for_each(vKeys.begin(), vKeys.end(), [&](int iKey) {
		if (GetAsyncKeyState(iKey))
		{
			mKeyInfos[iKey]->Set_PrevPressed(mKeyInfos[iKey]->Get_CurrentPressed());
			mKeyInfos[iKey]->Set_CurrentPressed(true);
		}
		else
		{
			mKeyInfos[iKey]->Set_CurrentPressed(false);
		}
		});
}

void CInputManager::Check_CursorPosition()
{
	GetCursorPos(&pCursor);
	ScreenToClient(g_hWnd, &pCursor);
}

const POINT& CInputManager::Get_CursorPosition()
{
	return pCursor;
}
