#include "pch.h"
#include "CFinalChest.h"
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


void CFinalChest::Render(HDC hDC)
{
	Vector2 vPos = pTransform->Position();
	if (vPos.Y() < vDroppedPos.Y())
	{
		pTransform->Position({ vPos.X(), vPos.Y() + 7 });
	}


	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"LastChest");

	if ((iAnimRow == 1 && iAnimCol < 2) && (dwDisplayElapsedTime + 100 < GetTickCount()))
	{
		dwDisplayElapsedTime = GetTickCount();
		iAnimCol++;
	}
	if (iAnimRow == 1 && iAnimCol == 2 && !bShowShotgun)
	{
		bShowShotgun = true;
		iAnimCol = 0;
		iAnimRow = 2;
	}

	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top(),
		(int)pRenderer->Size().X(), (int)pRenderer->Size().Y(),
		hMemDC,
		(int)iAnimCol * iRealSizeX,
		(int)iAnimRow * iRealSizeY,
		iRealSizeX, iRealSizeY,
		RGB(24, 24, 24));

	if (bShowShotgun)
	{
		Display_Shotgun(hDC);
	}
}

void CFinalChest::Late_Initialize()
{
	pTransform->Size({ 120, 120 });
	pCollider->Size({ 120, 120 });
	pRenderer->Size({ 150, 150 });

	iRealSizeX = iRealSizeY = 200;
	iAnimRow = 0;
	bShowShotgun = false;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Chest";

	rDetectBound = { -80, -80, 80, 80 };
}

void CFinalChest::Get_Item()
{
	dwDisplayElapsedTime = GetTickCount();
	iOffsetY = -20;
	iAnimRow = 1;
	iAnimCol = 0;
}

void CFinalChest::Apply_ItemEffect()
{
}

void CFinalChest::Drop_Item(Vector2 vDropPos)
{
	bDropEffect = true;
	vDroppedPos = { vDropPos.X(), vDropPos.Y() };
	pTransform->Position({ vDropPos.X(), vDropPos.Y() - 70.f });

	rDetectBound = { (int)vDropPos.X() + rDetectBound.left,(int)vDropPos.Y() + rDetectBound.top,
								(int)vDropPos.X() + rDetectBound.right, (int)vDropPos.Y() + rDetectBound.bottom };
}

void CFinalChest::OnDetect_PlayerIn()
{
	CItem::OnDetect_PlayerIn();

	if (iAnimRow == 0)
		iAnimCol = 1;
}

void CFinalChest::OnDetect_PlayerOut()
{
	if (iAnimRow == 0)
		iAnimCol = 0;
}

void CFinalChest::Display_Shotgun(HDC hDC)
{
	if (dwDisplayElapsedTime + 100000 < GetTickCount())
	{
		bShowShotgun = false;
	}

	HDC hGunDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Jusin");

	GdiTransparentBlt(hDC,
		pRenderer->Right() + 22, pRenderer->Top() + (--iOffsetY),
		100, 100,
		hGunDC,
		0, 0,
		150, 150,
		RGB(36, 36, 36));
	if (iOffsetY <= -70)
	{
		iOffsetY = -70;
	}
}
