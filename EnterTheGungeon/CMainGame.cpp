#pragma region INCLUDE
#include "pch.h"
#include "CMainGame.h"

#include "CManager.h"
#include "CInputManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CBmpManager.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CRelease.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CState.h"

#include "CBullet.h"
#include "CMouse.h"

#include "Vignette.h"
#pragma endregion


CMainGame::CMainGame()
{
	llElapsedTime = 0;
	iFPS = 0;
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

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Back/Back.bmp", L"Back");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Back/Tutorial.bmp", L"Tutorial");
	
	CObjectFactory<CMouse>::Create(O_UI);
	CBullet::Load_Resource();

	//pVignette = new Vignette;
	//pVignette->Initialize();
}

void CMainGame::Update()
{
	Compute_FPS();
	MANAGER(CInputManager*, M_INPUT)->Update();
	MANAGER(CSceneManager*, M_SCENE)->Update();
	MANAGER(CUIManager*, M_UI)->Update();
}

void CMainGame::Late_Update()
{
	MANAGER(CSceneManager*, M_SCENE)->Late_Update();
	MANAGER(CUIManager*, M_UI)->Late_Update();
}

void CMainGame::Render()
{
	if (MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene() == SC_INTRO)
		return;

	HDC hBackDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Back");
	MANAGER(CSceneManager*, M_SCENE)->Render(hBackDC);
	MANAGER(CUIManager*, M_UI)->Render(hBackDC);

	//pVignette->Render(hBackDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	for (UINT i = 0; i < MGREND; ++i)
	{
		CManager::Destroy_Manager((ManagerType)i);
	}
	// pVignette->Release();
	ReleaseDC(g_hWnd, hDC);
}

void CMainGame::Compute_FPS()
{
	++iFPS;

	if (llElapsedTime + 1000 < GetTickCount64())
	{
		swprintf_s(szFPS, L"FPS : %d", iFPS);
		iFPS = 0;

		SetWindowText(g_hWnd, szFPS);
		llElapsedTime = GetTickCount64();
	}
}
