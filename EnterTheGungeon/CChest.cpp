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
#include "CSoundManager.h";

void CChest::Render(HDC hDC)
{
	Vector2 vPos = pTransform->Position();
	if (vPos.Y() < vDroppedPos.Y())
	{
		pTransform->Position({ vPos.X(), vPos.Y() + 7 });
	}


	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Chest");

	if ((iAnimRow == 1 && iAnimCol < 2) && (dwDisplayElapsedTime + 100 < GetTickCount()))
	{
		dwDisplayElapsedTime = GetTickCount();
		iAnimCol++;
	}
	if (iAnimRow == 1 && iAnimCol == 2 && !bShowShotgun )
	{
		bShowShotgun = true;
		iAnimCol = 0;
		iAnimRow = 2;
		MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"Chest_Open.wav", 1.f);
	}

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
	iAnimRow = 1;
	iAnimCol = 0;
}

void CChest::Apply_ItemEffect()
{
}

void CChest::Drop_Item(Vector2 vDropPos)
{
	bDropEffect = true;
	vDroppedPos = { vDropPos.X(), vDropPos.Y() };
	pTransform->Position({ vDropPos.X(), vDropPos.Y() - 70.f});

	rDetectBound = { (int)vDropPos.X() + rDetectBound.left,(int)vDropPos.Y() + rDetectBound.top,
								(int)vDropPos.X() + rDetectBound.right, (int)vDropPos.Y() + rDetectBound.bottom };
}

void CChest::OnDetect_PlayerIn()
{
	CItem::OnDetect_PlayerIn();

	if (iAnimRow == 0) 
		iAnimCol = 1;
}

void CChest::OnDetect_PlayerOut()
{
	if (iAnimRow == 0) 
		iAnimCol = 0;
}

void CChest::Display_Shotgun(HDC hDC)
{
	if (dwDisplayElapsedTime + 1500 < GetTickCount())
	{
		bShowShotgun = false;
	}

	HDC hGunDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"GunType02");

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
