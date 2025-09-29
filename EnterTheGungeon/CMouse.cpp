#include "pch.h"
#include "CMouse.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CBmpManager.h"
#include "CInputManager.h"
#include "CCameraManager.h"
CMouse::CMouse()
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	CObject::Initialize();
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Cursor.bmp", L"Cursor");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/CursorBomb.bmp", L"CursorBomb");

	pTransform->Size({ 32.f, 32.f });
	pCollider->Size({ 32.f, 32.f });
	pRenderer->Size({ 32.f, 32.f });

	eType = O_UI;
	pRenderer->rType = RND__UI;

	ShowCursor(FALSE);

	Set_CursorMode(Normal);
}

int CMouse::Update()
{

	return 0;
}

void CMouse::Late_Update()
{
	CObject::Late_Update();
	Update_Transform();
}

void CMouse::Render(HDC _hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);

	GdiTransparentBlt(_hDC,
		pTransform->Position().X() - (int)pRenderer->Size().X() * 0.5f,
		pTransform->Position().Y() - (int)pRenderer->Size().Y() * 0.5f,
		iRenderSpriteX, iRenderSpriteY,
		hMemDC,
		0, 0,
		iRealSpriteX, iRealSpriteY,
		RGB(iRidColor, 0, iRidColor));
}

void CMouse::Update_Transform()
{
	POINT p = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	pTransform->Position({ (float)p.x, (float)p.y });
}

void CMouse::Set_CursorMode(CursorType type)
{
	switch (type)
	{
	case CMouse::Normal:
		spriteKey = L"Cursor";
		iRealSpriteX = iRealSpriteY = 32;
		iRenderSpriteX = iRenderSpriteY = 32;
		iRidColor = 255;
		break;
	case CMouse::Bomb:
		spriteKey = L"CursorBomb";
		iRealSpriteX = iRealSpriteY = 40;
		iRenderSpriteX = iRenderSpriteY = 40;
		iRidColor = 0;
		break;
	default:
		break;
	}
}
