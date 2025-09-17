#pragma once
class CScene
{
public :
	CScene() : eScene(SC_END) {};
	virtual ~CScene() {};

public :
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _hDC) = 0;
	virtual void Release() = 0;

public :
	SceneType Get_SceneType() { return eScene;  }

protected :
	SceneType eScene;
};

