#include "pch.h"
#include "CMouse.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CBmpManager.h"
#include "CInputManager.h"
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

	pTransform->Size({ 32.f, 32.f });
	pCollider->Size({ 32.f, 32.f });
	pRenderer->Size({ 32.f, 32.f });

	eType = O_UI;
	pRenderer->rType = RND__UI;

	ShowCursor(FALSE);
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
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Cursor");

	GdiTransparentBlt(_hDC,
		pTransform->Position().X() - (int)pRenderer->Size().X() * 0.5f,
		pTransform->Position().Y() - (int)pRenderer->Size().Y() * 0.5f,
		(int)pRenderer->Size().X(),
		(int)pRenderer->Size().Y(),
		hMemDC,
		0, 0,
		(int)pRenderer->Size().X(),
		(int)pRenderer->Size().Y(),
		RGB(255, 0, 255));
}

void CMouse::Update_Transform()
{
	POINT p = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	pTransform->Position({ (float)p.x, (float)p.y });
}
