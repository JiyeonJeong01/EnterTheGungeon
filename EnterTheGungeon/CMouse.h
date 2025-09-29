#pragma once
#include "CObject.h"
class CMouse : public CObject
{
public :
	enum CursorType { Normal, Bomb };
public :
	CMouse();
	~CMouse() override;

public :
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() {};

	void Update_Transform() override;

public :
	void Set_CursorMode(CursorType type);

private :
	const TCHAR* spriteKey = L"";
	int iRealSpriteX, iRealSpriteY;
	int iRenderSpriteX, iRenderSpriteY;
	int iRidColor;
};

