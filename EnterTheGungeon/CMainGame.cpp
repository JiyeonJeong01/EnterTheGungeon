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
#include "CSoundManager.h"
#include "CStageManager.h"
#include "CRelease.h"

#include "CResourceLoader.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CState.h"

#include "CMouse.h"

#include "Vignette.h"
#pragma endregion


CMainGame::CMainGame()
{
	llElapsedTime = 0;
	iFPS = iFPSCounter = 0;
	hDC = NULL;
	bFrameUpdated = true;
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	llElapsedTime = GetTickCount64();
	hDC = GetDC(g_hWnd);

	CResourceLoader::Load_Resource();
	MANAGER(CUIManager*, M_UI)->Initialize();
	MANAGER(CInputManager*, M_INPUT)->Initialize();
	MANAGER(CObjectManager*, M_OBJECT)->Initialize();
	MANAGER(CSceneManager*, M_SCENE)->Initialize();
	MANAGER(CSoundManager*, M_SOUND)->Initialize();
	//MANAGER(CStageManager*, M_STAGE)->Initialize();

	CObjectFactory<CMouse>::Create(O_UI);

	MANAGER(CSoundManager*, M_SOUND)->PlayBGM(L"BGM_Boss.wav", 0.35f);

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

	SetStretchBltMode(hDC, COLORONCOLOR);
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
	POINT pPos = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 vRealPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)pPos.x, (float)pPos.y });

	++iFPSCounter;
	if ((unsigned)(llElapsedTime + 1000 )< GetTickCount64())
	{
		iFPS = iFPSCounter;
		iFPSCounter = 0;
		llElapsedTime = GetTickCount64();
	}

	swprintf_s(szFPS, L"FPS : %d\t, Cursor Pos : {%ld, %ld}, Real Pos : {%d, %d}", 	iFPS, pPos.x, pPos.y, (int)vRealPos.X(), (int)vRealPos.Y());

	SetWindowText(g_hWnd, szFPS);
}
