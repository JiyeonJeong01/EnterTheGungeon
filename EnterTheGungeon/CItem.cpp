#include "pch.h"
#include "CItem.h"
#include "CBmpManager.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CInputManager.h"
#include "CInventory.h"
#include "CBossState.h"
#include "CSoundManager.h"

DWORD CItem::dwLastPurchasedTime = 0;
bool CItem::bFree = false;

CItem::~CItem()
{
	Release();
}

void CItem::Load_Resource()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Coin.bmp", L"Coin");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Cartridge.bmp", L"Cartridge");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Bomb.bmp", L"Bomb");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Coin_UI.bmp", L"Coin_UI");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Cartridge_UI.bmp", L"Cartridge_UI");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Chest.bmp", L"Chest");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Medkit.bmp", L"Medkit");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Boomerang.bmp", L"Boomerang");
}

void CItem::Initialize()
{
	CObject::Initialize();

	bCanUse = false;		// Can be used only after the player obtains this item
	bCanInteract = false;
	bObtained = false;
	iAnimCol = iAnimRow =0;

	bAlive = true;

	bForSell = bDisplayInfo = false;
	bDisplayPopup = bDisplaySuccessPopup = bDisplayFailPopup = false;

	eType = O_ITEM;
	pRenderer->rType = RND__GAMEBOJECT;

	Late_Initialize();
	
	pPlayer = nullptr;
	bDisplayPressE = false;
	iPressKeyAnimCol = 0;
}

int CItem::Update()
{
	if (!bAlive) return S_DEAD;

	if (bObtained) return 0;
	Detect_Player();

	return 0;
}

void CItem::Late_Update()
{
	CObject::Update_Collider();
	CObject::Update_Renderer();
}

void CItem::Render(HDC hDC)
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
	if (!bForSell && bObtained) return;
	if (bDisplayPressE)
	{
		Show_EKey(hDC);
	}
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);

	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top(),
		(int)pRenderer->Size().X(), (int)pRenderer->Size().Y(),
		hMemDC, 
		(int)iAnimCol* iRealSizeX, iAnimRow,
		iRealSizeX, iRealSizeY,
		RGB(38, 38, 38));
}

void CItem::Release()
{

}

void CItem::Detect_Player()
{
	if (pPlayer == nullptr)
	{
		pPlayer = static_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
		if(pPlayer == nullptr) return;
	}

	RECT rCollisionArea{};

	if (IntersectRect(&rCollisionArea, &rDetectBound, pPlayer->Get_Collider()->Get_PCollider()))
	{				
		OnDetect_PlayerIn();
	}
	else
	{
		OnDetect_PlayerOut();
	}
}

void CItem::OnDetect_PlayerIn()
{
	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E'))
	{
		Get_Item();
	}
}

/// <summary>
/// Derived objects must initialize rDetectBound, spriteKey, iAnimSizeX, iAnimSizeY, transform, renderer;
/// </summary>
void CItem::Late_Initialize()
{
}

void CItem::Drop_Item(Vector2 vDropPos)
{
	pTransform->Position(move(vDropPos));
	if (bForSell)
	{
		rDetectBound = { rDetectBound.left / 2, rDetectBound.top, rDetectBound.right / 2, (int)(rDetectBound.bottom * 2.5f) };
	}
	rDetectBound = { (int)vDropPos.X() + rDetectBound.left,(int)vDropPos.Y() + rDetectBound.top,
								(int)vDropPos.X() + rDetectBound.right, (int)vDropPos.Y() + rDetectBound.bottom };

}

void CItem::Get_Item()
{
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"web_sfx_get-coin.mp3", 1.f);
	bObtained = true;
	pPlayer->pInventory->Add_Item(spriteKey, this);
}

void CItem::Apply_ItemEffect()
{
}


void CItem::OnDetect_PlayerOut()
{
	iAnimCol = 0;
}

void CItem::Show_EKey(HDC hDC)
{
	if (dwPressKeyElapsedTime + 200 < GetTickCount())
	{
		dwPressKeyElapsedTime = GetTickCount();
		iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
	}
	int iRealSizeX = 32, iRealSizeY = 32;
	int iRenderSizeX = 40, iRenderSizeY = 40;

	HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"EKey");

	GdiTransparentBlt(hDC, pRenderer->Left() + 75, pRenderer->Top() - 95, iRenderSizeX, iRenderSizeY,
		hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
}

void CItem::Show_Guide_Success(HDC hDC)
{
	HDC hInfoDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Dialogue");

	int iRenderSizeX = 490, iRenderSizeY = 130;
	int posX = (WINCX >> 1) - (iRenderSizeX) / 2 + 80, posY = WINCY - 125;
	GdiTransparentBlt(hDC,
		posX, posY,
		iRenderSizeX, iRenderSizeY,
		hInfoDC,
		0, 0,
		iPopupRealSizeX, iPopupRealSizeY,
		RGB(53, 53, 53));

	HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
	);

	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	TCHAR buffer1[64];
	if (bFree == false)
	{
		swprintf_s(buffer1, 64, L"아이템을 구매했습니다!");
	}
	else
	{
		swprintf_s(buffer1, 64, L"아이템을 빼앗았습니다...");
		posX -= 4;
	}
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, posX + 47, posY + 27, buffer1, lstrlen(buffer1));

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}

void CItem::Show_Guide_Fail(HDC hDC)
{
	HDC hInfoDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Dialogue");

	int iRenderSizeX = 490, iRenderSizeY = 130;
	int posX = (WINCX >> 1) - (iRenderSizeX) / 2 + 80, posY = WINCY - 125;
	GdiTransparentBlt(hDC,
		posX, posY,
		iRenderSizeX, iRenderSizeY,
		hInfoDC,
		0, 0,
		iPopupRealSizeX, iPopupRealSizeY,
		RGB(53, 53, 53));

	HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
	);

	HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

	TCHAR buffer1[64];
	swprintf_s(buffer1, 64, L"코인이 부족합니다.");
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, posX + 67, posY + 27, buffer1, lstrlen(buffer1));

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}
