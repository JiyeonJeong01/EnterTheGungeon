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
	pCurrentScene->Update();
}

void CSceneManager::Late_Update()
{
	pCurrentScene->Late_Update();
}

void CSceneManager::Render(HDC hDC)
{
	pCurrentScene->Render(hDC);
}

void CSceneManager::Release()
{
	pCurrentScene->Release();
	CRelease<CScene*>::Release(pCurrentScene);
}

void CSceneManager::Change_Scene(SceneType sType)
{
	if (eCurrentScene == sType) return;

	delete pCurrentScene;

	switch (sType)
	{
	case SC_INTRO : 
		pCurrentScene = new CIntroScene;
		break;
	case SC_MAIN :
		//pCurrentScene = new CMainScene;
		break;
	case SC_TEST:
		pCurrentScene = new CTestSCene;
		break;
	}
	pCurrentScene->Initialize();
	eCurrentScene = sType;
}

void CSceneManager::Change_SceneEffect()
{


}
