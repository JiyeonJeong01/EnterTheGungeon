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
#include "CStageManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CMapManager.h"
#include "CBmpManager.h"
#include "CTableObject.h"
#include "CCartridge.h"
#include "CCoin.h"
#include "CBomb.h"
#include "CBoss.h"
#include "CTeleport.h"
#include "CEndingGuide.h"
#include "CFinalChest.h"
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

	Place_Objects();
	MANAGER(CStageManager*, M_STAGE)->Initialize_BossStage();
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
	MANAGER(CStageManager*, M_STAGE)->Logic_BossStage();
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
	Draw_Teleport(_hDC);
	MANAGER(CUIManager*, M_UI)->Render(_hDC);
}

void CTestSCene::Release()
{
	// MANAGER(CObjectManager*, M_OBJECT)->Release();
}

void CTestSCene::Place_Objects()
{
	// 테스트용 아이템 배치

}

void CTestSCene::Draw_Teleport(HDC hDC)
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

	GdiTransparentBlt(hDC, vRenderPos.X(), vRenderPos.Y(), 200, 200, tableDC, iAnimCol * iRealSize, 0, iRealSize, iRealSize, RGB(55, 55, 55));
}
