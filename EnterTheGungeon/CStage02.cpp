#pragma region INCLUDE
#include "pch.h"
#include "CStage02.h"

#include "CManager.h"
#include "CObjectManager.h"
#include "CCameraManager.h"
#include "CCollisionManager.h"
#include "CMapManager.h"
#include "CBmpManager.h"
#include "CStageManager.h"
#include "CObjectFactory.h"

#include "CPlayer.h"

#include "CMap.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

#include "CTableObject.h"
#include "CTeleport.h"
#pragma endregion

CStage02::CStage02()
{
	pPlayer = nullptr;
}

CStage02::~CStage02()
{
	Release();
}

void CStage02::Initialize()
{
	eScene = SC_STAGE02;
	POINT pPlayerPos = { 630, 482 };

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

	MANAGER(CEnvironmentManager*, M_MAP)->Initialize();
	MANAGER(CCameraManager*, M_CAMERA)->Set_LookAt({ (float)pPlayerPos.x, (float)pPlayerPos.y });
	MANAGER(CCameraManager*, M_CAMERA)->Set_Target(pPlayer);

	bDrawTeleport = false;

	MANAGER(CStageManager*, M_STAGE)->Initialize_Stage02();

	Place_Objects();
}

void CStage02::Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Update();
	MANAGER(CEnvironmentManager*, M_MAP)->Update();

	Detect_Collision();

	MANAGER(CCameraManager*, M_CAMERA)->Update();
	MANAGER(CStageManager*, M_STAGE)->Logic_Stage02();
}

void CStage02::Late_Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Late_Update();
}

void CStage02::Render(HDC _hDC)
{
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ 0.f, 0.f });
	HDC hBackground = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Stage02");

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

void CStage02::Release()
{
}

void CStage02::Detect_Collision()
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

void CStage02::Set_TeleportOn()
{
	dwAnimElapsedTime = GetTickCount();
	bDrawTeleport = true;
	iAnimCol = 0;
}

void CStage02::Place_Objects()
{
	for (Vector2 vPos : vTablePos01)
	{
		CTableObject* pTable = static_cast<CTableObject*>(
			CObjectFactory<CTableObject>::Create(O_INTERACTABLE, vPos.X(), vPos.Y()));
		MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList()->push_back(pTable);
	}
}

void CStage02::Draw_Teleport(HDC hDC)
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
