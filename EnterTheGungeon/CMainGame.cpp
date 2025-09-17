#pragma region INCLUDE
#include "pch.h"
#include "CMainGame.h"

#include "CManager.h"
#include "CInputManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CObjectFactory.h"
#include "CRelease.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CState.h"
#pragma endregion


CMainGame::CMainGame()
{
	llElapsedTime = 0;
	hDC = NULL;
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	llElapsedTime = GetTickCount64();
	hDC = GetDC(g_hWnd);

	MANAGER(CInputManager*, M_INPUT)->Initialize();
	MANAGER(CObjectManager*, M_OBJECT)->Initialize();
	MANAGER(CSceneManager*, M_SCENE)->Initialize();
}

void CMainGame::Update()
{
	MANAGER(CInputManager*, M_INPUT)->Update();
	MANAGER(CSceneManager*, M_SCENE)->Update();
}

void CMainGame::Late_Update()
{
	MANAGER(CSceneManager*, M_SCENE)->Late_Update();
}

void CMainGame::Render()
{
	// HDC hBackDC = 
	Rectangle(hDC, 0, 0, WINCX, WINCY);
	MANAGER(CSceneManager*, M_SCENE)->Render(hDC);
}

void CMainGame::Release()
{
	for (UINT i = 0; i < MGREND; ++i)
	{
		CManager::Destroy_Manager((ManagerType)i);
	}

	ReleaseDC(g_hWnd, hDC);
}
