#include "pch.h"
#include "CUIManager.h"

#include "CObject.h"
#include "CRelease.h"

#include "CObjectManager.h"
#include "CInputManager.h"
#include "CBmpManager.h"
#include "CSTLHelper.h"

#include "CPlayer.h"
#include "CInventory.h"
#include "CItem.h"
#include "CBoss.h"

#include "CMouse.h"

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
	bBossDraw = false;
	bDrawPlayer = false;
	dwEffecctTime = GetTickCount();
	iEffectIndex = 0;
	bOpenInventory = false;
	iWheelRange = 100;
}

void CUIManager::Update()
{
	for (auto& ui : uiObjects)
	{
		ui->Update();
	}
	if (bDrawPlayer && pInventory == nullptr)
	{
		this->pInventory = static_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front())->pInventory;
		if (pInventory == nullptr) return;
	}

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
	if (bDrawPlayer)
	{
		Draw_BasicInventory(hDC);

		if (!MANAGER(CInputManager*, M_INPUT)->Get_Key(VK_LSHIFT) && bOpenInventory)
		{
			bOpenInventory = false;
			if (bActivatedSlotTurn == false)
			{
				// 활성화된 아이템 바꿔주는 로직 !!
				pInventory->Set_CurActiveItem(curDisplayIter->first);
			}
		}

		if (MANAGER(CInputManager*, M_INPUT)->Get_Key(VK_LSHIFT))
		{
			if (bOpenInventory == false)
				Prepare_OpenInventory(); /// 열기 전 사전 작업

			Draw_OpenedInventory(hDC);
		}
		Draw_ClosedInventory(hDC);
	}
	if (bBossDraw)
	{
		Draw_BossStat(hDC);
	}

	Draw_CartridgeEffect(hDC);
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
	if (dynamic_cast<CMouse*>(pObj) != nullptr)
	{
		pMouse = static_cast<CMouse*>(pObj);
	}
	uiObjects.push_back(pObj);
}

void CUIManager::Draw_BasicInventory(HDC hDC)
{
	int iX = 20, iY = 65;
	int iSize = 27;
	iCartridge = pInventory->Get_Cartridge();
	iCoin = pInventory->Get_Coin();

	HDC hCartridge = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Cartridge_UI");
	HDC hCoin = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Coin_UI");
	HDC hKey = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Key_UI");

	GdiTransparentBlt(hDC, iX, iY, iSize, iSize, hCartridge, 0, 0, iSize, iSize, RGB(38, 38, 38));
	GdiTransparentBlt(hDC, iX + iSize * 2, iY, iSize, iSize, hCoin, 0, 0, iSize, iSize, RGB(38, 38, 38));

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
}

void CUIManager::Draw_ClosedInventory(HDC hDC)
{
	int iFrameSizeX = 142, iFrameSizeY = 88;
	int iFrmaeX = 30, iFrameY = 60 + iFrameSizeY;
	if (bActivatedSlotTurn)
	{
		int iOffset = 3;
		HDC hItemFrame = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WeaponType_Active");
		GdiTransparentBlt(hDC, iFrmaeX, WINCY - iFrameY, iFrameSizeX + iOffset, iFrameSizeY + iOffset, hItemFrame, 0, 0, iFrameSizeX, iFrameSizeY, RGB(0, 0, 0));
	}
	else
	{
		HDC hItemFrame = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WeaponType");
		GdiTransparentBlt(hDC, iFrmaeX, WINCY - iFrameY, iFrameSizeX, iFrameSizeY, hItemFrame, 0, 0, iFrameSizeX, iFrameSizeY, RGB(0, 0, 0));
	}

	CItem* curItem = pInventory->Get_CurActiveItem();
	int iOffset = bActivatedSlotTurn ? 2 : 0;
	if (curItem != nullptr)
	{
		HDC hItemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(curItem->Get_ItemKey());
		GdiTransparentBlt(hDC, iFrmaeX + 42, WINCY - iFrameY + iOffset + 10, 60, 60, hItemDC, 40, 0, 40, 40, RGB(38, 38, 38));

		auto iter = find_if(begin(pInventory->Get_ItemMap()), end(pInventory->Get_ItemMap()), [&](const pair< const TCHAR*, list<CItem*>>& p)->bool
			{
				return (lstrcmp(p.first, curItem->Get_ItemKey()) == 0);
			});

		int iItemCount = iter->second.size();

		// 아이템 수량 텍스트
		int textOffsetX = (iFrmaeX + 116), textOffsetY = (WINCY - iFrameY + iOffset + 52);
		HFONT hFont = CreateFont(
			18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
		);

		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

		TCHAR buffer[64];
		swprintf_s(buffer, 64, _T("%d"), iItemCount);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, textOffsetX, textOffsetY, buffer, lstrlen(buffer));

		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
}

void CUIManager::Draw_OpenedInventory(HDC hDC)
{
	// 스크롤 시 하이라이트된 슬롯 변경하기 위한 입력 받기
	if (iWheelScroll > 0 && (dwLastWheelElapsedTime + iWheelRange < GetTickCount()))
	{
		dwLastWheelElapsedTime = GetTickCount();
		if (displayInventory.empty())
			return;
		if (!bActivatedSlotTurn) // 평상시 동작
		{
			if (curDisplayIter == displayInventory.end())
			{
				bActivatedSlotTurn = true;
			}
			else
			{
				++curDisplayIter;
				if (curDisplayIter == displayInventory.end())
				{
					bActivatedSlotTurn = true;
				}
			}
		}
		else
		{
			bActivatedSlotTurn = false;
			if (curDisplayIter == displayInventory.end())
				curDisplayIter = displayInventory.begin();
		}
	}

#pragma region ui 값
	int iFrameSizeX = 142, iFrameSizeY = 88;
	int iFrmaeX = 30, iFrameY = 60 + iFrameSizeY;
	int iSmallFrameSizeX = 100, iSmallFrameSizeY = 60;
	int iSmallFrameRenderX = iFrmaeX, iSmallFrameRenderY = WINCY - iFrameY - iSmallFrameSizeY;
	int renderIndex = 0;
#pragma endregion

	HDC hItemFrame = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WeaponType");
	HDC hActiveItemFrame = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WeaponType_Active");

	for (auto& pair : displayInventory)
	{
		const TCHAR* key = pair.first;
		const auto& items = pair.second;

		if (items.empty())
			continue;

		int iItemCount = items.size();

		int frameX = iSmallFrameSizeX;
		int frameY = iSmallFrameSizeY;
		int renderX = iSmallFrameRenderX;
		int renderY = iSmallFrameRenderY - 5 - (renderIndex * (iSmallFrameSizeY + 6));

		HDC hFrameDC = hItemFrame; 

		if (curDisplayIter != displayInventory.end())
		{
			if (!bActivatedSlotTurn) // 활성화된 아이템이 하이라이트되지 않은 경우
			{
				// 활성화된 슬롯 
				if (wcscmp(curDisplayIter->first, key) == 0)
				{
					frameX = static_cast<int>(iSmallFrameSizeX * 1.2f);
					frameY = static_cast<int>(iSmallFrameSizeY * 1.2f);
					renderY -= (frameY - iSmallFrameSizeY) / 2;
					hFrameDC = hActiveItemFrame;
				}
			}
		}

		// 프레임 그리기
		GdiTransparentBlt(hDC,
			renderX, renderY, frameX, frameY,
			hFrameDC,
			0, 0, iFrameSizeX, iFrameSizeY,
			RGB(0, 0, 0));

		HDC hItemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(key);

		int itemWidth = 40, itemHeight = 40;
		int itemOffsetX = (frameX - itemWidth) / 2, itemOffsetY = (frameY - itemHeight) / 2;

		// 아이템 그리기
		GdiTransparentBlt(hDC,
			renderX + itemOffsetX,  renderY + itemOffsetY,
			itemWidth, itemHeight,
			hItemDC,
			itemWidth, 0,
			itemWidth, itemHeight,
			RGB(38, 38, 38));

		// 아이템 수량 텍스트
		int textOffsetX = (itemOffsetX + 49), textOffsetY = (itemOffsetY + 25);
		HFONT hFont = CreateFont(
			14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
		);

		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

		TCHAR buffer[64];
		swprintf_s(buffer, 64, _T("%d"), iItemCount);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, renderX + textOffsetX, renderY + textOffsetY, buffer, lstrlen(buffer));

		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);


		renderIndex++;
	}
}

void CUIManager::Draw_CartridgeEffect(HDC hDC)
{
	if (bShouldCartridgeAnim && !bEndCartridgeAnim)
	{
		if (dwCartridgeAnim + 40 < GetTickCount())
		{
			iCartridgeCol++;
			dwCartridgeAnim = GetTickCount();

			if (iCartridgeCol > iCartridgeColMax)
			{
				if (iCartridgeRow == 0)
				{
					iCartridgeCol = 0;
					iCartridgeRow = 1;
				}
				else
				{
					bEndCartridgeAnim = true;
				}
			}
		}

		HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Cartridge_Effect");
		this;

		GdiTransparentBlt(hDC,
			(WINCX >> 1) - iCartridgeSize,
			(WINCY >> 1) - iCartridgeSize,
			iCartridgeSize*2, iCartridgeSize*2,
			hMemDC,
			iCartridgeCol * iCartridgeSize,
			iCartridgeRow * iCartridgeSize,
			iCartridgeSize, iCartridgeSize,
			RGB(32, 32, 32));
	}
}

void CUIManager::Start_CartridgeEffect()
{
	bShouldCartridgeAnim = true;
	bEndCartridgeAnim = false;
	iCartridgeCol = iCartridgeRow = 0;
	dwCartridgeAnim = GetTickCount();
}

void CUIManager::Prepare_OpenInventory()
{
	bOpenInventory = true;
	bActivatedSlotTurn = true;
	dwLastWheelElapsedTime = GetTickCount();
	displayInventory.clear();

	int iCurrentIndex = 0;
	int iItemTypeCount = 3; // 테스트 !

	for (auto item : pInventory->Get_ItemMap())
	{
		const auto& key = item.first;
		if (key == nullptr) continue;


		if (wcscmp(key, L"Coin") == 0 || wcscmp(key, L"Cartridge") == 0)
			continue;

		if (pInventory->Get_CurActiveItem() != nullptr && pInventory->Get_CurActiveItem()->Get_ItemKey() == key)
			continue;

		int itemCount = item.second.size();
		if (itemCount > 0)
		{
			iItemTypeCount++;
			displayInventory.insert(item);
		}

		iCurrentIndex++;
	}
	
	baseDisplayIter =  displayInventory.begin();
	curDisplayIter = displayInventory.end();
}

void CUIManager::Draw_BossStat(HDC hDC)
{
	if (!bBossDraw) return;

	int iX = 661, iY = 60;

	HDC hBar = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_HPBar");
	HDC hEffect = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_HP_Effect");
	GdiTransparentBlt(hDC, WINCX / 2 - iX/2, WINCY - iY - iY, iX, iY, hBar, 0, 0, iX, iY, RGB(0, 0, 0));
	//Rectangle(hDC, WINCX / 2 - iX / 2, WINCY - iY + 30, WINCX / 2 + iX / 2, WINCY - iY + 30);


	int iBarL = 375, iBarT = 620, iBarR = 907, iBarB = 642;
	int iLen = iBarR - iBarL;
	int iHPLen = pBoss->iMaxHP - pBoss->iHP;
	int iNewL = iBarR - (float)iHPLen * 0.01f * iLen;
	if (iNewL <= iBarL) iNewL = iBarL;

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Rectangle(hDC, iNewL, iBarT, iBarR, iBarB);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);

	int iEffectX = 64, iEffectY = 64;
	GdiTransparentBlt(hDC, iNewL - iEffectX/2 + 10, iBarT-iEffectY/2+10, iEffectX, iEffectY, hEffect, iEffectIndex * iEffectX, 0, iEffectX, iEffectY, RGB(0, 0, 0));
	if (dwEffecctTime + 100 < GetTickCount())
	{
		dwEffecctTime = GetTickCount();
		iEffectIndex++;
		if (iEffectIndex > 5)
		{
			iEffectIndex = 0;
		}
	}

}