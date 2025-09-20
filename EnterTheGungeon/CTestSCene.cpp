#include "pch.h"
#include "CTestSCene.h"
#include "CManager.h"
#include "CObjectManager.h"
#include "CObjectFactory.h"
#include "CPlayer.h"
#include "CCameraManager.h"
#include "CMapCollider.h"
#include "CCollisionManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CButton.h"
#include "CMapManager.h"
#include "CBmpManager.h"

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
	pPlayer = dynamic_cast<CPlayer*>(CObjectFactory<CPlayer>::Create(O_PLAYER, WINCX >> 1, WINCY >> 1));
	MANAGER(CEnvironmentManager*, M_MAP)->Initialize();
	MANAGER(CCameraManager*, M_CAMERA)->Set_LookAt({ WINCX >> 1, WINCY >> 1 });
	MANAGER(CCameraManager*, M_CAMERA)->Set_Target(pPlayer);
}

void CTestSCene::Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Update();
	MANAGER(CEnvironmentManager*, M_MAP)->Update();

	// Map ground <-> Player
	CCollisionManager::Detect_MapCollision(
		*MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList(), 
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER));

	// Map ground <-> Player Bullet
	CCollisionManager::Detect_MapCollision(
		*MANAGER(CEnvironmentManager*, M_MAP)->Get_MapGroundList(),
		*MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLBULLET));
	MANAGER(CCameraManager*, M_CAMERA)->Update();
}

void CTestSCene::Late_Update()
{
	MANAGER(CObjectManager*, M_OBJECT)->Late_Update();
}

void CTestSCene::Render(HDC _hDC)
{
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({0.f, 0.f});
	HDC hBackground = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Tutorial");
	BitBlt(_hDC, 0, 0, WINCX, WINCY, hBackground, -vRenderPos.X(), -vRenderPos.Y(), SRCCOPY);

	MANAGER(CObjectManager*, M_OBJECT)->Render(_hDC);
	for_each(mapList.begin(), mapList.end(), [&](CMapCollider* map)->void {map->Render(_hDC); });
	MANAGER(CEnvironmentManager*, M_MAP)->Render(_hDC);
}

void CTestSCene::Release()
{
	// MANAGER(CObjectManager*, M_OBJECT)->Release();
}
