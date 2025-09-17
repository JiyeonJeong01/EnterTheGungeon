#pragma region INCLUDE
#include "pch.h"
#include "CSceneManager.h"

#include "CManager.h"
#include "CObjectManager.h"
#include "CRelease.h"

#include "CScene.h"
#include "CIntroScene.h"
#include "CMainScene.h"
#include "CTestSCene.h"

#include "CObject.h"
#include "CPlayer.h"
#pragma endregion

CSceneManager::CSceneManager()
{
	pCurrentScene = nullptr;
	pPrecScene = nullptr;
	eCurrentScene = SC_END;
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Initialize()
{
	Change_Scene(SC_TEST);
}

void CSceneManager::Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Update();
}

void CSceneManager::Late_Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Late_Update();
}

void CSceneManager::Render(HDC hDC)
{
	MANAGER(CObjectManager*, M_OBJECT)->Render(hDC);
}

void CSceneManager::Release()
{
	// MANAGER(CObjectManager*, M_OBJECT)->Release();
	CRelease<CScene*>::Release(pCurrentScene);
}

void CSceneManager::Change_Scene(SceneType sType)
{
	if (eCurrentScene == sType) return;

	switch (sType)
	{
	case SC_INTRO : 
		//pCurrentScene = new CIntroScene;
		break;
	case SC_MAIN :
		//pCurrentScene = new CMainScene;
		break;
	case SC_TEST:
		pCurrentScene = new CTestSCene;
		break;
	}
	pCurrentScene->Initialize();
}
