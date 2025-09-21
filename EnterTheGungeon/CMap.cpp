#include "pch.h"
#include "CMap.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CCollider.h"

#include "CBmpManager.h"

CMap::CMap()
{
}

CMap::~CMap()
{
	Release();
}

void CMap::Load_Resource()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Table_IDLE.bmp", L"Table_IDLE");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Table_UP.bmp", L"Table_UP");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Table_DOWN.bmp", L"Table_DOWN");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Table_LEFT.bmp", L"Table_LEFT");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Table_RIGHT.bmp", L"Table_RIGHT");

}

void CMap::Initialize()
{
	CObject::Initialize();
}

int CMap::Update()
{
	return 0;
}

void CMap::Render(HDC hDC)
{
	CObject::Update_Renderer();
	CObject::Render(hDC);
}

void CMap::Release()
{
}

void CMap::OnCollision(CObject* pObj, Vector2 vDiff)
{
}

void CMap::OnCollision_Entity(CObject * pObj, Vector2 vDiff)
{
}
