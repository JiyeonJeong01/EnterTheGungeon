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

void CItem::Load_Resource()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Coin.bmp", L"Coin");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Cartridge.bmp", L"Cartridge");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Bomb.bmp", L"Bomb");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Coin_UI.bmp", L"Coin_UI");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Cartridge_UI.bmp", L"Cartridge_UI");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Chest.bmp", L"Chest");
}

void CItem::Initialize()
{
	CObject::Initialize();

	bCanUse = false;		// Can be used only after the player obtains this item
	bCanInteract = false;
	bObtained = false;
	iAnimCol = 0;

	eType = O_ITEM;
	pRenderer->rType = RND__GAMEBOJECT;

	Late_Initialize();
	
	pPlayer = nullptr;
}

int CItem::Update()
{
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
	if (bObtained) return;
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);

	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top(),
		(int)pRenderer->Size().X(), (int)pRenderer->Size().Y(),
		hMemDC, 
		(int)iAnimCol* iRealSizeX, 0,
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
		bCanInteract = true;
		iAnimCol = 1;
		
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
	rDetectBound = { (int)vDropPos.X() + rDetectBound.left,(int)vDropPos.Y() + rDetectBound.top,
								(int)vDropPos.X() + rDetectBound.right, (int)vDropPos.Y() + rDetectBound.bottom };
}

void CItem::Get_Item()
{
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
