#pragma once
#include "CScene.h"
class CIntroScene : public CScene
{
public :
	CIntroScene();
	virtual ~CIntroScene();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	HWND hVideo;
};
