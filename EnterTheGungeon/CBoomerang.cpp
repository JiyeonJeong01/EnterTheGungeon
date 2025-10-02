#include "pch.h"
#include "CBoomerang.h"

#include "CObject.h"
#include "CMob.h"
#include "CBoss.h"

#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CInputManager.h"
#include "CRelease.h"

void CBoomerang::Late_Initialize()
{
	pTransform->Size({ 40, 40 });
	pCollider->Size({ 40, 40 });
	pRenderer->Size({ 60, 60 });

	iRealSizeX = iRealSizeY = 40;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();

	spriteKey = L"Boomerang";

	rDetectBound = { -40, -40, 40, 40 };
	bBoomerangWorking = false;
	bBoomerangReturn = false;
	iPrice = 700;
}

int CBoomerang::Update()
{
	if (bObtained && !bBoomerangWorking) return 0;

	Detect_Player();

	if (bBoomerangWorking)
	{
		Attack_Enemies();
	}


	return 0;
}

void CBoomerang::Render(HDC hDC)
{
	if (bForSell)
	{
		Display_ItemInfo(hDC);
	}

	if (bDisplaySuccessPopup)
	{
		Show_Guide_Success(hDC);
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('M'))
			bDisplaySuccessPopup = false;
	}

	if (bDisplayFailPopup)
	{
		Show_Guide_Fail(hDC);
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('M'))
			bDisplayFailPopup = false;
	}

	// 팔렸으면 그리면 안 되는 부분 
	if (!bBoomerangWorking && !bForSell && bObtained) return;
	if (bDisplayPressE)
	{
		Show_EKey(hDC);
	}
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);

	if (bBoomerangWorking && dwAnimElapsedTime + 100 < GetTickCount())
	{
		dwAnimElapsedTime = GetTickCount();

		iAnimCol++;
		if (iAnimCol >= 2) iAnimCol = 0;
	}

	if (bBoomerangWorking)
	{
		Render_Path(hDC);
	}

 	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top(),
		(int)pRenderer->Size().X(), (int)pRenderer->Size().Y(),
		hMemDC,
		(int)iAnimCol * iRealSizeX, iAnimRow * iRealSizeX,
		iRealSizeX, iRealSizeY,
		RGB(38, 38, 38));
 }

void CBoomerang::Get_Item()
{
	CItem::Get_Item();

	if (bForSell) {
		bForSell = false;
		bDisplayPressE = false;
		Vector2 vPos = pTransform->Position();
		CBoomerang* pBoomerang = static_cast<CBoomerang*>(CObjectFactory<CBoomerang>::Create(O_ITEM));
		pBoomerang->Set_ForSell(true);
		pBoomerang->Drop_Item({ vPos.X() , vPos.Y() });
		dwLastPurchasedTime = GetTickCount();
	}
}

void CBoomerang::Apply_ItemEffect()
{
	Vector2 vPlayer = pPlayer->Get_Transform()->Position();

	int iDist = 1000;
	RECT rDetect = { (int)vPlayer.X() - iDist,  (int)vPlayer.Y() - iDist,  (int)vPlayer.X() + iDist,  (int)vPlayer.Y() + iDist };

	pDetectedEnemies.clear();
	// 범위 안에 있는 적들 받아오기 
	for_each(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENEMY)->begin(),
		MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENEMY)->end(),
		[&](CObject* pObj) -> void {
			if (pObj->Get_Transform()->Position().X() >= rDetect.left &&
				pObj->Get_Transform()->Position().X() <= rDetect.right &&
				pObj->Get_Transform()->Position().Y() >= rDetect.top &&
				pObj->Get_Transform()->Position().X() <= rDetect.bottom)
			{
				pDetectedEnemies.push_back(pObj);
			}});
			if (pDetectedEnemies.size() > 0)
			{
				bBoomerangWorking = true;
				itCurEnemy = pDetectedEnemies.begin();
				pTransform->Position(move( vPlayer ));
			}
			dwAnimElapsedTime = GetTickCount();
			iAnimRow = 1;
}

void CBoomerang::Display_ItemInfo(HDC hDC)
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
	swprintf_s(buffer1, 64, L"부메랑");
	swprintf_s(buffer2, 64, L"600 코인");
	swprintf_s(buffer3, 64, L"모든 적들에게 타격!!");
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, posX + 25, posY + 23, buffer1, lstrlen(buffer1));
	TextOut(hDC, posX + 25, posY + 39, buffer2, lstrlen(buffer2));
	TextOut(hDC, posX + 25, posY + 54, buffer3, lstrlen(buffer3));

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void CBoomerang::OnDetect_PlayerIn()
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

void CBoomerang::OnDetect_PlayerOut()
{
	if (bForSell)
	{
		bDisplayInfo = false;
		bDisplayPressE = false;
	}
	iAnimCol = 0;
}

void CBoomerang::Attack_Enemies()
{
	float fSpeed = 15.f;

	Vector2 vEnemyPosition = (*itCurEnemy)->Get_Transform()->Position();
	Vector2 vBoomPositoin = pTransform->Position();
	Vector2 vDirection = vEnemyPosition - vBoomPositoin;
	float fDist = vDirection.Get_Magnitude();
	vDirection.Normalize();

	pTransform->Position({ vBoomPositoin.X() + vDirection.X() * fSpeed,
											vBoomPositoin.Y() + vDirection.Y() * fSpeed });


	if (fDist < 20.f)
	{

		if (dynamic_cast<CMob*>(*itCurEnemy) != nullptr)
		{
			static_cast<CMob*>(*itCurEnemy)->bKnockback = true;
			static_cast<CMob*>(*itCurEnemy)->Modify_HP(-2);
		}
		else
		{
			// 보스
			static_cast<CBoss*>(*itCurEnemy)->iHP -= 10;
		}
		itCurEnemy++;
		if (itCurEnemy == pDetectedEnemies.end())
		{
			bBoomerangWorking = false;
			bBoomerangReturn = true;

		}
	}


}

void CBoomerang::Render_Path(HDC hDC)
{
	Vector2 vCurPos = pTransform->Position();
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(vCurPos);

	POINT curPoint = { (int)vRenderPos.X(), (int)vRenderPos.Y() };

	HPEN hPen = CreatePen(PS_DASHDOT, 1, RGB(255, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	MoveToEx(hDC, curPoint.x, curPoint.y, NULL);
	list<CObject*>::iterator itTmp = itCurEnemy;
	for (; itTmp != pDetectedEnemies.end(); ++itTmp)
	{
		if (!(*itTmp)) continue;

		Vector2 vEnemyPos = (*itTmp)->Get_Transform()->Position();
		Vector2 vEnemyRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(vEnemyPos);
		POINT enemyPoint = { (int)vEnemyRenderPos.X(), (int)vEnemyRenderPos.Y() };
		LineTo(hDC, enemyPoint.x, enemyPoint.y);
	}

	// 펜 원상복구
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}
