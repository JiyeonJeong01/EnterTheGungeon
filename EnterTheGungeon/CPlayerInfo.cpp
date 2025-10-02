#include "pch.h"
#include "CPlayerInfo.h"
#include "CBmpManager.h"
#include "CPlayer.h"
#include "CCameraManager.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CInputManager.h"
#include "CPlayerWeapon.h"
#include "CWeapon.h"

CPlayerInfo::CPlayerInfo()
{
}

CPlayerInfo::~CPlayerInfo()
{
	Release();
}



void CPlayerInfo::Initialize()
{
	CObject::Initialize();
	pRenderer->rType = RND__UI;

	pTransform->Size({ 45.f, 45.f });
	pCollider->Size({ 45.f, 45.f });
	pRenderer->Size({ 45.f, 45.f });
}

int CPlayerInfo::Update()
{
	iPlayerHP = pPlayer->iHP;

	return 0;
}

void CPlayerInfo::Late_Update()
{
}

void CPlayerInfo::Render(HDC hDC)
{
	HDC hHPDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"HP");

	// HP
	Draw_HP(hDC);
	Draw_WeaponType(hDC);
	Draw_BulletCount(hDC);
}

void CPlayerInfo::Release()
{
}

void CPlayerInfo::Update_Renderer()
{
}

void CPlayerInfo::Draw_HP(HDC hDC)
{
	int iFirstHP, iSecondHP, iThirdHP;

	switch (iPlayerHP)
	{
	case 6: 
		iFirstHP = 0; iSecondHP = 0; iThirdHP = 0;
		break;
	case 5:
		iFirstHP = 0; iSecondHP = 0; iThirdHP = 1;
		break;
	case 4:
		iFirstHP = 0; iSecondHP = 0; iThirdHP = 2;
		break;
	case 3:
		iFirstHP = 0; iSecondHP = 1; iThirdHP = 2;
		break;
	case 2:
		iFirstHP = 0; iSecondHP = 2; iThirdHP = 2;
		break;
	case 1:
		iFirstHP = 1; iSecondHP = 2; iThirdHP = 2;
		break;
	case 0: default :
		iFirstHP = 2; iSecondHP = 2; iThirdHP = 2;
		break;
	}

	int iX = 20, iY = 20;
	int iSize = 38;

	HDC hHPDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"HP");

	GdiTransparentBlt(hDC, iX, iY, iSize, iSize, hHPDC,	iFirstHP * iSize, 0, iSize, iSize,	RGB(30, 30, 30));
	GdiTransparentBlt(hDC, iX + iSize + 5, iY, iSize, iSize, hHPDC, iSecondHP * iSize, 0, iSize, iSize, RGB(30, 30, 30));
	GdiTransparentBlt(hDC, iX + iSize * 2 + 10, iY, iSize, iSize, hHPDC, iThirdHP * iSize,0, iSize, iSize, RGB(30, 30, 30));
}

void CPlayerInfo::Draw_WeaponType(HDC hDC)
{
	int iX = 90, iY = 60;
	int iSizeX = 142;
	int iSizeY = 88;

	HDC hHPDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WeaponType");

	GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iSizeY, iSizeX, iSizeY, hHPDC, 0, 0, iSizeX, iSizeY, RGB(0, 0, 0));
	if (pPlayer->pWeapon->Get_WeaponType() == CWeapon::PG01)
	{
		int iGunSizeX = 94;
		int iGunSizeY = 55;
		int iBlankX = 25;
		int iBlankY = 15;
		HDC hGunDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"GunType01");
		GdiTransparentBlt(hDC, WINCX - iX - iSizeX + iBlankX, WINCY - iY - iSizeY + iBlankY, iGunSizeX, iGunSizeY, hGunDC, 0, 0, iGunSizeX, iGunSizeY, RGB(0, 0, 0));

	}
	else
	{
		int iGunSizeX = 100;
		int iGunSizeY = 31;
		int iBlankX = 20;
		int iBlankY = 25;
		HDC hGunDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"GunType02");
		GdiTransparentBlt(hDC, WINCX - iX - iSizeX + iBlankX, WINCY - iY - iSizeY + iBlankY, iGunSizeX, iGunSizeY, hGunDC, 0, 0, iGunSizeX, iGunSizeY, RGB(0, 0, 0));
	}

}

void CPlayerInfo::Draw_BulletCount(HDC hDC)
{
	int iX = 40, iY = 60;
	int iSizeX = 20;
	int iBulletSizeY = 4;
	int iBlankSizeY = 3;

	int iBarSizeY = 10;
	int iCurrent = iBarSizeY + iBlankSizeY * 4;


	if (pPlayer->pWeapon->Get_WeaponType() == CWeapon::PG01)
	{
		int iSizeY = 100;
		iBulletCount = pPlayer->pWeapon->Get_CurPistolBullet();
		HDC hBarDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"BulletBar1");
		HDC hBulletDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"BulletCount");

		GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iSizeY, iSizeX, iSizeY, hBarDC, 0, 0, iSizeX, iSizeY, RGB(0, 0, 0));

		for (int i = 0; i < iBulletCount; i++)
		{
			GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iCurrent, iSizeX, iBulletSizeY, hBulletDC, 0, 0, iSizeX, iBulletSizeY, RGB(0, 0, 0));
			iCurrent = iCurrent + iBulletSizeY + iBlankSizeY;
		}
		for (int i = iBulletCount; i < 10; i++)
		{
			GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iCurrent, iSizeX, iBulletSizeY, hBulletDC, 0, 5, iSizeX, iBulletSizeY, RGB(0, 0, 0));
			iCurrent = iCurrent + iBulletSizeY + iBlankSizeY;
		}
	}
	else
	{
		int iSizeY = 70;
		iY += 10;

		iBulletCount = pPlayer->pWeapon->Get_CurShotgunBullet();
		HDC hBarDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"BulletBar2");
		HDC hBulletDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"BulletCount");

		GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iSizeY, iSizeX, iSizeY, hBarDC, 0, 0, iSizeX, iSizeY, RGB(0, 0, 0));

		for (int i = 0; i < iBulletCount; i++)
		{
			GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iCurrent, iSizeX, iBulletSizeY, hBulletDC, 0, 0, iSizeX, iBulletSizeY, RGB(0, 0, 0));
			iCurrent = iCurrent + iBulletSizeY + iBlankSizeY;
		}
		for (int i = iBulletCount; i < 5; i++)
		{
			GdiTransparentBlt(hDC, WINCX - iX - iSizeX, WINCY - iY - iCurrent, iSizeX, iBulletSizeY, hBulletDC, 0, 5, iSizeX, iBulletSizeY, RGB(0, 0, 0));
			iCurrent = iCurrent + iBulletSizeY + iBlankSizeY;
		}
	}
}

void CPlayerInfo::Shake()
{


}

void CPlayerInfo::Load_Resource()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/HP0.bmp", L"HP");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/WeaponType.bmp", L"WeaponType");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/BulletBar.bmp", L"BulletBar1");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/BulletBar2.bmp", L"BulletBar2");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/BulletCount.bmp", L"BulletCount");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/GunType1.bmp", L"GunType01");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/GunType2.bmp", L"GunType02");
}