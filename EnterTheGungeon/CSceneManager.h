#pragma once
#include "CManager.h"

class CScene;

class CSceneManager : public CManager
{
public :
	CSceneManager();
	virtual ~CSceneManager() override;

public: // Core
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC);
	void Release() override;

public :
	void Change_Scene(SceneType sType);

private :
	CScene* pCurrentScene;
	CScene* pPrecScene;

	SceneType eCurrentScene;
};

