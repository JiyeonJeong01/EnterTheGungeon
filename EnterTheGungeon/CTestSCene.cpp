#pragma region INCLUDE
#include "pch.h"
#include "CTestSCene.h"
#include "CManager.h"
#include "CObjectManager.h"
#include "CObjectFactory.h"
#include "CPlayer.h"
#include "CCameraManager.h"
#include "CMap.h"
#include "CCollisionManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CButton.h"
#include "CMapManager.h"
#include "CBmpManager.h"
#include "CTableObject.h"
#include "CCartridge.h"
#include "CCoin.h"
#include "CBomb.h"
#include "CBoss.h"
#include "CTeleport.h"
#include "CMob01.h"
#include "CMob02.h"
#include "CMob03.h"
#include "CMob04.h"
#include "CMob05.h"
#pragma endregion


CTestSCene::CTestSCene()
{
	pPlayer = nullptr;
}

CTestSCene::~CTestSCene()
{
	Release();
}

void CTestSCene::Initialize()
{
	eScene = SC_TEST;

	if (pPlayer == nullptr)
	{
		if (MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->empty())
		{
			pPlayer = dynamic_cast<CPlayer*>(CObjectFactory<CPlayer>::Create(O_PLAYER, pPlayerPos.x, pPlayerPos.y));
		}
		else
		{
			pPlayer = dynamic_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
			pPlayer->Get_Transform()->Position({ (float)pPlayerPos.x , (float)pPlayerPos.y });
		}
	}

	 pBoss = dynamic_cast<CBoss*>(CObjectFactory<CBoss>::Create(O_ENEMY, pBossPos.x, pBossPos.y));

	MANAGER(CEnvironmentManager*, M_MAP)->Initialize();
	MANAGER(CCameraManager*, M_CAMERA)->Set_LookAt({ (float)pPlayerPos.x, (float)pPlayerPos.y });
	MANAGER(CCameraManager*, M_CAMERA)->Set_Target(pPlayer);

	// 테스트용 테이블 생성 
	CTableObject* pTable1 = dynamic_cast<CTableObject*>(
		CObjectFactory<CTableObject>::Create(O_INTERACTABLE, pBossPos.x + 150, pBossPos.y - 200));
	MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable1);

	CTableObject* pTable2 = dynamic_cast<CTableObject*>(
		CObjectFactory<CTableObject>::Create(O_INTERACTABLE, pBossPos.x - 220, pBossPos.y + 100));
	MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable2);

	CTableObject* pTable3 = dynamic_cast<CTableObject*>(
		CObjectFactory<CTableObject>::Create(O_INTERACTABLE, pBossPos.x + 300, pBossPos.y + 180));
	MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable3);

	//CTeleport* pTeleport = dynamic_cast<CTeleport*>(
	//	CObjectFactory<CTeleport>::Create(O_INTERACTABLE, 2800, 2650));
	//MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTeleport);

	//// 테스트용 아이템 생성w
	CCartridge* pItem1 = dynamic_cast<CCartridge*>(CObjectFactory<CCartridge>::Create(O_ITEM, pBossPos.x + 100, pBossPos.y + 120));
	pItem1->Drop_Item({ (float)(pBossPos.x + 100), (float)(pBossPos.y + 120) });

	CCoin* pItem2 = dynamic_cast<CCoin*>(CObjectFactory<CCoin>::Create(O_ITEM, pBossPos.x - 80, pBossPos.y + 50));
	pItem2->Drop_Item({ (float)(pBossPos.x - 80), (float)(pBossPos.y + 50) });

	CBomb* pItem3 = dynamic_cast<CBomb*>(CObjectFactory<CBomb>::Create(O_ITEM, pBossPos.x + 60, pBossPos.y - 70));
	pItem3->Drop_Item({ (float)(pBossPos.x + 60), (float)(pBossPos.y - 70) });

	CCartridge* pItem4 = dynamic_cast<CCartridge*>(CObjectFactory<CCartridge>::Create(O_ITEM, pBossPos.x - 120, pBossPos.y - 50));
	pItem4->Drop_Item({ (float)(pBossPos.x - 120), (float)(pBossPos.y - 50) });

	CCoin* pItem5 = dynamic_cast<CCoin*>(CObjectFactory<CCoin>::Create(O_ITEM, pBossPos.x + 50, pBossPos.y - 130));
	pItem5->Drop_Item({ (float)(pBossPos.x + 50), (float)(pBossPos.y - 130) });
	//CBomb* pItem6 = dynamic_cast<CBomb*>(CObjectFactory<CBomb>::Create(O_ITEM, 1020, 880));
	//pItem6->Drop_Item({ 1020, 880 });

}

void CTestSCene::Update()
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
	MANAGER(CUIManager*, M_UI)->Update();
}

void CTestSCene::Late_Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Late_Update();
}

void CTestSCene::Render(HDC _hDC)
{
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({0.f, 0.f});
	HDC hBackground = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Stage03");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hBackground, -vRenderPos.X(), -vRenderPos.Y(), SRCCOPY);
	MANAGER(CObjectManager*, M_OBJECT)->Render(_hDC);
	MANAGER(CEnvironmentManager*, M_MAP)->Render(_hDC);

	MANAGER(CUIManager*, M_UI)->Render(_hDC);
}

void CTestSCene::Release()
{
	// MANAGER(CObjectManager*, M_OBJECT)->Release();
}
