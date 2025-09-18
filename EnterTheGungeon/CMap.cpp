#include "pch.h"
#include "CMap.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CCollider.h"

CMap::CMap()
{

}

CMap::CMap(const RECT& rect)
{
	rBound.left = rect.left; 
	rBound.top = rect.top;
	rBound.right = rect.right;
	rBound.bottom = rect.bottom;
}

CMap::~CMap()
{
	Release();
}

void CMap::Initialize()
{

}

void CMap::Render(HDC hDC)
{
	Rectangle(hDC, rBound.left, rBound.top, rBound.right, rBound.bottom);
}

void CMap::Release()
{

}