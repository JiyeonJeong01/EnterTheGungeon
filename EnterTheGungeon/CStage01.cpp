#pragma region INCLUDE
#include "pch.h"
#include "CStage01.h"

#include "CObjectManager.h"
#include "CCameraManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CStageManager.h"
#include "CMapManager.h"
#include "CBmpManager.h"

#include "CObjectFactory.h"

#include "CMap.h"
#include "CPlayer.h"
#include "CGuide.h"

#include "CButton.h"
#include "CCoin.h"
#include "CBomb.h"

#include "CTeleport.h"
#include "CTableObject.h"
#include "CCartridge.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CStage02.h"
#pragma endregion

CStage01::CStage01()
{
	pPlayer = nullptr;
}

CStage01::~CStage01()
{
	Release();
}

void CStage01::Initialize()
{
	eScene = SC_STAGE01;
	POINT pPlayerPos = { 4918, 810 };
	pPlayer = dynamic_cast<CPlayer*>(CObjectFactory<CPlayer>::Create(O_PLAYER, pPlayerPos.x, pPlayerPos.y));
	CObjectFactory<CGuide>::Create(O_ENEMY, pPlayerPos.x + 100, pPlayerPos.y + 500);

	MANAGER(CEnvironmentManager*, M_MAP)->Initialize();
	MANAGER(CCameraManager*, M_CAMERA)->Set_LookAt({ (float)pPlayerPos.x, (float)pPlayerPos.y });
	MANAGER(CCameraManager*, M_CAMERA)->Set_Target(pPlayer);

	bDrawTeleport = false;

	//  테이블 생성 
	Place_Objects();

	MANAGER(CStageManager*, M_STAGE)->Initialize_Stage01();
}

void CStage01::Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Update();
	MANAGER(CEnvironmentManager*, M_MAP)->Update();

	Detect_Collision();

	MANAGER(CCameraManager*, M_CAMERA)->Update();
	MANAGER(CStageManager*, M_STAGE)->Logic_Stage01();
}

void CStage01::Late_Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Late_Update();
}

void CStage01::Render(HDC _hDC)
{
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ 0.f, 0.f });
	HDC hBackground = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Stage01");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hBackground, -vRenderPos.X(), -vRenderPos.Y(), SRCCOPY);
	
	if (bDrawTeleport)
		Draw_Teleport(_hDC);

	MANAGER(CObjectManager*, M_OBJECT)->Render(_hDC);
	MANAGER(CEnvironmentManager*, M_MAP)->Render(_hDC);

	MANAGER(CUIManager*, M_UI)->Render(_hDC);

#pragma region DEBUG
	MANAGER(CStageManager*, M_STAGE)->Render(_hDC);
#pragma endregion

}

void CStage01::Release() { }

void CStage01::Set_TeleportOn()
{
	dwAnimElapsedTime = GetTickCount();
	bDrawTeleport = true;
	iAnimCol = 0;
}

void CStage01::Detect_Collision()
{
	// Map ground <-> Player
	CCollisionManager::Detect_MapCollision(
		*MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList(),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER));

	// Map ground <-> Enemy
	CCollisionManager::Detect_MapCollision(
		*MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList(),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENEMY));

	// Map ground <-> Player Bullet
	CCollisionManager::Detect_MapCollision(
		*MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList(),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLBULLET));

	// Map object <-> Enemy Bullet
	CCollisionManager::Detect_MapCollision(
		*MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList(),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENBULLET));

	// Player <-> Enemy Bullet
	CCollisionManager::Detect_RectCollision(
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENBULLET));

	// Enemy <-> Player Bullet
	CCollisionManager::Detect_RectCollision(
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENEMY),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLBULLET));

}

void CStage01::Place_Objects()
{
	for (Vector2 vPos : vTablePos01)
	{
		CTableObject* pTable = static_cast<CTableObject*>(
			CObjectFactory<CTableObject>::Create(O_INTERACTABLE, vPos.X(), vPos.Y()));
		MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable);
	}

	for (Vector2 vPos : vTablePos02)
	{
		CTableObject* pTable = static_cast<CTableObject*>(
			CObjectFactory<CTableObject>::Create(O_INTERACTABLE, vPos.X(), vPos.Y()));
		MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable);
	}


	// 테스트용 아이템 생성 
	POINT pPlayerPos = { 4918, 810 };
	CItem* pItem1 = static_cast<CItem*>(CObjectFactory<CBomb>::Create(O_ITEM));
	pItem1->Drop_Item({ (float)pPlayerPos.x, (float)pPlayerPos.y });
	CItem* pItem2 = static_cast<CItem*>(CObjectFactory<CBomb>::Create(O_ITEM));
	pItem2->Drop_Item({ (float)pPlayerPos.x -100, (float)pPlayerPos.y -100 });
	CItem* pItem3 = static_cast<CItem*>(CObjectFactory<CCartridge>::Create(O_ITEM));
	pItem3->Drop_Item({ (float)pPlayerPos.x -200, (float)pPlayerPos.y });
	CItem* pItem4 = static_cast<CItem*>(CObjectFactory<CCartridge>::Create(O_ITEM));
	pItem4->Drop_Item({ (float)pPlayerPos.x - 50, (float)pPlayerPos.y - 70 });

	
}

void CStage01::Draw_Teleport(HDC hDC)
{
	if (dwAnimElapsedTime + 200 < GetTickCount())
	{
		dwAnimElapsedTime = GetTickCount();
		iAnimCol++;
		if (iAnimCol > 3) iAnimCol = 0;
	}

	HDC tableDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Teleport_Opened");

	int iRenderSize = 200;
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(vRealPos);

	GdiTransparentBlt(hDC, 	vRenderPos.X(), vRenderPos.Y(), 	200, 200, 	tableDC, 	iAnimCol * iRealSize, 0, 	iRealSize, iRealSize, RGB(55, 55, 55));
}
