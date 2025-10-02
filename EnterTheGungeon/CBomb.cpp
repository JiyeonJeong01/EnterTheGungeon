#include "pch.h"
#include "CBomb.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CInputManager.h"
#include "CUIManager.h"
#include "CRelease.h"

#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

#include "CInventory.h"
#include "CMouse.h"

void CBomb::Late_Initialize()
{
	pTransform->Size({ 40, 40 });
	pCollider->Size({ 40, 40 });
	pRenderer->Size({ 40, 40 });

	iRealSizeX = iRealSizeY = 40;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Bomb";

	rDetectBound = { -30, -40, 30, 40 };

	iPrice = 300;

}

void CBomb::Get_Item()
{
	CItem::Get_Item();

	if (bForSell)
	{
		bForSell = false;
		bDisplayPressE = false;

		Vector2 vPos = pTransform->Position();
		CBomb* pBomb = static_cast<CBomb*>(CObjectFactory<CBomb>::Create(O_ITEM));
		pBomb->Set_ForSell(true);
		pBomb->Drop_Item({ vPos.X() , vPos.Y() });

		dwLastPurchasedTime = GetTickCount();
	}
}

void CBomb::Apply_ItemEffect()
{
	MANAGER(CUIManager*, M_UI)->Get_Mouse()->Set_CursorMode(CMouse::Bomb);

	pPlayer->Set_ShotMode(CPlayer::PlayerAttack::Bomb);
	printf("used\n");
}

void CBomb::Display_ItemInfo(HDC hDC)
{
	if (!bDisplayInfo) return;

	Vector2 vRender = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(pTransform->Position());

	HDC hInfoDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Info");

	int posX = vRender.X() + 50, posY = vRender.Y() - 80;

	GdiTransparentBlt(hDC,
		posX, posY,
		160, 90,
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
	swprintf_s(buffer1, 64, L"폭탄");
	swprintf_s(buffer2, 64, L"300 코인");
	swprintf_s(buffer3, 64, L"적에게 강한 데미지!!");
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, posX + 25, posY + 23, buffer1, lstrlen(buffer1));
	TextOut(hDC, posX + 25, posY + 39, buffer2, lstrlen(buffer2));
	TextOut(hDC, posX + 25, posY + 54, buffer3, lstrlen(buffer3));

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void CBomb::OnDetect_PlayerIn()
{
	bCanInteract = true;
	iAnimCol = 1;
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
		if (dwLastPurchasedTime + 300 >= GetTickCount()) return;
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

void CBomb::OnDetect_PlayerOut()
{
	if (bForSell)
	{
		bDisplayInfo = false;
		bDisplayPressE = false;
	}
	iAnimCol = 0;

}
