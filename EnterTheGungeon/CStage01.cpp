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
#include "CBoss.h"

#include "CButton.h"
#include "CCoin.h"
#include "CBomb.h"

#include "CTeleport.h"
#include "CTableObject.h"
#include "CCartridge.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
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

	MANAGER(CEnvironmentManager*, M_MAP)->Initialize();
	MANAGER(CCameraManager*, M_CAMERA)->Set_LookAt({ (float)pPlayerPos.x, (float)pPlayerPos.y });
	MANAGER(CCameraManager*, M_CAMERA)->Set_Target(pPlayer);


	// 테스트용 테이블 생성 
	//CTableObject* pTable1 = dynamic_cast<CTableObject*>(
	//	CObjectFactory<CTableObject>::Create(O_INTERACTABLE, pBossPos.x + 150, pBossPos.y - 200));
	//MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable1);

	//CTableObject* pTable2 = dynamic_cast<CTableObject*>(
	//	CObjectFactory<CTableObject>::Create(O_INTERACTABLE, pBossPos.x - 220, pBossPos.y + 100));
	//MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable2);

	//CTableObject* pTable3 = dynamic_cast<CTableObject*>(
	//	CObjectFactory<CTableObject>::Create(O_INTERACTABLE, pBossPos.x + 300, pBossPos.y + 180));
	//MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable3);

	//CTeleport* pTeleport = dynamic_cast<CTeleport*>(
	//	CObjectFactory<CTeleport>::Create(O_INTERACTABLE, 2800, 2650));
	//MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTeleport);

	//// 테스트용 아이템 생성w
	//CCartridge* pItem1 = dynamic_cast<CCartridge*>(CObjectFactory<CCartridge>::Create(O_ITEM, 900, 900));
	//pItem1->Drop_Item({ 900, 900 });

	//CCoin* pItem2 = dynamic_cast<CCoin*>(CObjectFactory<CCoin>::Create(O_ITEM, 800, 900));
	//pItem2->Drop_Item({ 800, 900 });

	//CBomb* pItem3 = dynamic_cast<CBomb*>(CObjectFactory<CBomb>::Create(O_ITEM, 1000, 900));
	//pItem3->Drop_Item({ 1000, 900 });

	//CCartridge* pItem4 = dynamic_cast<CCartridge*>(CObjectFactory<CCartridge>::Create(O_ITEM, 920, 880));
	//pItem4->Drop_Item({ 920, 880 });

	//CCoin* pItem5 = dynamic_cast<CCoin*>(CObjectFactory<CCoin>::Create(O_ITEM, 820, 880));
	//pItem5->Drop_Item({ 820, 880 });

	//CBomb* pItem6 = dynamic_cast<CBomb*>(CObjectFactory<CBomb>::Create(O_ITEM, 1020, 880));
	//pItem6->Drop_Item({ 1020, 880 });

	MANAGER(CStageManager*, M_STAGE)->Initialize_Stage01();

}

void CStage01::Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Update();
	MANAGER(CEnvironmentManager*, M_MAP)->Update();

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
	MANAGER(CObjectManager*, M_OBJECT)->Render(_hDC);
	MANAGER(CEnvironmentManager*, M_MAP)->Render(_hDC);

	MANAGER(CUIManager*, M_UI)->Render(_hDC);

#pragma region DEBUG
	MANAGER(CStageManager*, M_STAGE)->Render(_hDC);
#pragma endregion

}

void CStage01::Release()
{

}