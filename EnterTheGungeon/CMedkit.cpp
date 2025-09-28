#include "pch.h"
#include "CMedkit.h"
#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CInputManager.h"
#include "CRelease.h"

#include "CInventory.h"
#include "CPlayer.h"

void CMedkit::Late_Initialize()
{
	pTransform->Size({ 40, 40 });
	pCollider->Size({ 40, 40 });
	pRenderer->Size({ 40, 40 });

	iRealSizeX = iRealSizeY = 40;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Medkit";

	rDetectBound = { -30, -40, 30, 40 };

	iPrice = 100;
}

void CMedkit::Get_Item()
{
	CItem::Get_Item();

	bForSell = false;
	bDisplayPressE = false;

	Vector2 vPos = pTransform->Position();
	CMedkit* pMedkit = static_cast<CMedkit*>(CObjectFactory<CMedkit>::Create(O_ITEM));
	pMedkit->Set_ForSell(true);
	pMedkit->Drop_Item({ vPos.X() , vPos.Y() });

	dwLastPurchasedTime = GetTickCount();
}

void CMedkit::Apply_ItemEffect()
{
	pPlayer->iHP = (pPlayer->iHP+1 > pPlayer->iMaxHP ? pPlayer->iMaxHP : pPlayer->iHP + 1);
}

void CMedkit::Display_ItemInfo(HDC hDC)
{
	if (!bDisplayInfo) return;

	Vector2 vRender = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(pTransform->Position());

	HDC hInfoDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Info");

	int posX = vRender.X() + 50, posY = vRender.Y() - 80;

	GdiTransparentBlt(hDC,
		posX, posY,
		130, 90,
		hInfoDC,
		0, 0,
		iInfoRealSizeX, iInfoRealSizeY,
		RGB(53, 53, 53));

	HFONT hFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
	);

	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	TCHAR buffer1[64];
	TCHAR buffer2[64];
	TCHAR buffer3[64];
	swprintf_s(buffer1, 64, L"구급상자");
	swprintf_s(buffer2, 64, L"100 코인");
	swprintf_s(buffer3, 64, L"체력 즉시 회복!!");
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, posX + 25, posY + 23, buffer1, lstrlen(buffer1));
	TextOut(hDC, posX + 25, posY + 39, buffer2, lstrlen(buffer2));
	TextOut(hDC, posX + 25, posY + 54, buffer3, lstrlen(buffer3));

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void CMedkit::OnDetect_PlayerIn()
{
	if (bForSell)
	{
		bDisplayInfo = true;
		if (!bDisplayPressE)
		{
			iPressKeyAnimCol = 0;
			dwPressKeyElapsedTime = GetTickCount();
		}
		bDisplayPressE = true;
	}

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E'))
	{
		if (dwLastPurchasedTime + 800 >= GetTickCount()) return;
		if (bForSell)
		{
			bDisplayPopup = true;
			int iPlayerCoin = pPlayer->pInventory->Get_Coin();
			if (bFree)
			{
				Get_Item();
				bDisplaySuccessPopup = true;
			}
			else if (iPrice > iPlayerCoin)
			{
				bDisplayFailPopup = true;
			}
			else
			{
				bDisplaySuccessPopup = true;
				pPlayer->pInventory->Minus_Coin(iPrice);
				Get_Item();
			}
		}
		else
		{
			Get_Item();
		}
	}
}

void CMedkit::OnDetect_PlayerOut()
{
	if (bForSell)
	{
		bDisplayInfo = false;
		bDisplayPressE = false;
	}
	iAnimCol = 0;
}
