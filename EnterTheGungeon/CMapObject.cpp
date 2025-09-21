#include "pch.h"
#include "CMapObject.h"

CMapObject::CMapObject()
{
}

CMapObject::~CMapObject()
{
	Release();
}

void CMapObject::Initialize()
{
	CMap::Initialize();
}

int CMapObject::Update()
{
	return 0;
}

void CMapObject::Render(HDC hDC)
{
	CMap::Render(hDC);
}

void CMapObject::Release()
{
}

void CMapObject::OnCollision(CObject* pObj)
{

}

void CMapObject::OnCollision_Entity(CObject* pObj)
{

}