#include "pch.h"
#include "CMapCollider.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CCollider.h"

CMapCollider::CMapCollider(const RECT& rect)
{
	rBound.left = rect.left; 
	rBound.top = rect.top;
	rBound.right = rect.right;
	rBound.bottom = rect.bottom;
}

CMapCollider::~CMapCollider()
{
	Release();
}

void CMapCollider::Initialize()
{

}

void CMapCollider::Render(HDC hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	Rectangle(hDC, rBound.left, rBound.top, rBound.right, rBound.bottom);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void CMapCollider::Release()
{

}