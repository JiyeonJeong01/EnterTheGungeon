#include "pch.h"
#include "CChest.h"

#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CRelease.h"

#include "CPlayerBullet.h"
#include "CBossBullet.h"


void CChest::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Chest");

	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top(),
		(int)pRenderer->Size().X(), (int)pRenderer->Size().Y(),
		hMemDC,
		(int)iAnimCol * iRealSizeX,
		(int)iAnimRow * iRealSizeY,
		iRealSizeX, iRealSizeY,
		RGB(27, 27, 27));

	if (bShowShotgun)
	{
		Display_Shotgun(hDC);
	}
}

void CChest::Late_Initialize()
{
	pTransform->Size({ 120, 120 });
	pCollider->Size({ 120, 120 });
	pRenderer->Size({ 120, 120 });

	iRealSizeX = iRealSizeY = 200;
	iAnimRow = 0;
	bShowShotgun = false;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Chest";

	rDetectBound = { -100, -100, 100, 100 };
}

void CChest::Get_Item()
{
	dwDisplayElapsedTime = GetTickCount();
	iOffsetY = - 20;
	bShowShotgun = true;
}

void CChest::Apply_ItemEffect()
{
}

void CChest::OnDetect_PlayerIn()
{
	CItem::OnDetect_PlayerIn();

	if (iAnimRow == 0) iAnimCol = 1;
}

void CChest::OnDetect_PlayerOut()
{
	if (iAnimRow == 0) iAnimCol = 0;
}

void CChest::Display_Shotgun(HDC hDC)
{
	if (dwDisplayElapsedTime + 1500 < GetTickCount())
	{
		bShowShotgun = false;
	}

	HDC hGunDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"GunType02");

	if (!hGunDC)
	{
		MessageBox(NULL, L"Error: Failed to load image 'GunType2'", L"Image Load Error", MB_ICONERROR);
	}

	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top() + (--iOffsetY),
		100, 31, 
		hGunDC,
		0, 0, 
		100, 31,
		RGB(0, 0, 0));
	if (iOffsetY <= -70)
	{
		iOffsetY = -70;
	}
}
