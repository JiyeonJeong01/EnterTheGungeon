#include "pch.h"
#include "CUIManager.h"

#include "CObject.h"
#include "CRelease.h"

#include "CObjectManager.h"
#include "CBmpManager.h"
#include "CSTLHelper.h"

#include "CPlayer.h"
#include "CInventory.h"
#include "CItem.h"

CUIManager::CUIManager()
{
	pInventory = nullptr;
}

CUIManager::~CUIManager()
{
	Release();
}

void CUIManager::Initialize()
{
}

void CUIManager::Update()
{
	for (auto& ui : uiObjects)
	{
		ui->Update();
	}
	if (pInventory == nullptr)
	{
		this->pInventory = static_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front())->pInventory;
		if (pInventory == nullptr) return;
	}
	iCartridge = pInventory->Get_Cartridge();
	iCoin = pInventory->Get_Coin();
}

void CUIManager::Late_Update()
{
	for (auto& ui : uiObjects)
	{
		ui->Late_Update();
	}
}

void CUIManager::Render(HDC hDC)
{
	for (auto& ui : uiObjects)
	{
		ui->Render(hDC);
	}
	Draw_Inventory(hDC);
}

void CUIManager::Release()
{
	for (auto& ui : uiObjects)
	{
		CRelease<CObject*>::Release(ui);
	}
}

void CUIManager::Add_Object(CObject* pObj)
{
	if (pObj == nullptr) return;

	uiObjects.push_back(pObj);
}

void CUIManager::Draw_Inventory(HDC hDC)
{
#pragma region Basic item info
	int iX = 20, iY = 65;
	int iSize = 27;

	HDC hCartridge = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Cartridge_UI");
	HDC hCoin = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Coin_UI");
	HDC hKey = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Key_UI");

	GdiTransparentBlt(hDC, iX, iY, iSize, iSize, hCartridge, 0, 0, iSize, iSize, RGB(38, 38, 38));
	GdiTransparentBlt(hDC, iX + iSize * 2, iY, iSize, iSize, hCoin, 0, 0, iSize, iSize, RGB(38, 38, 38));

	// TODO : 키 꼭 구현하기 

	// Text
	HFONT hFont = CreateFont(
		18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
	);

	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	TCHAR buffer[64];
	swprintf_s(buffer, 64, _T("%d"), iCartridge);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, iX + iSize + 4, iY + 4, buffer, lstrlen(buffer));

	TCHAR buffer2[64];
	swprintf_s(buffer2, 64, _T("%d"), iCoin);
	TextOut(hDC, iX + iSize * 3 + 6, iY + 4, buffer2, lstrlen(buffer2));


	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
#pragma endregion

#pragma region ItemType
	int iFrameSizeX = 142, iFrameSizeY = 88;
	int iFrmaeX = 30, iFrameY = 60 + iFrameSizeY;
	HDC hItemFrame = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WeaponType");

	GdiTransparentBlt(hDC, iFrmaeX, WINCY - iFrameY, iFrameSizeX, iFrameSizeY, hItemFrame, 0, 0, iFrameSizeX, iFrameSizeY, RGB(0, 0, 0));

	CItem* curItem = pInventory->Get_CurActiveItem();
	if (curItem != nullptr)
	{
		HDC hItemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(curItem->Get_ItemKey());
		GdiTransparentBlt(hDC, iFrmaeX + 50, WINCY - iFrameY + 20, 40, 40, hItemDC, 40, 0, 40, 40, RGB(38, 38, 38));
	}

#pragma endregion

};