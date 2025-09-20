#include "pch.h"
#include "CMapCollider.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CCollider.h"

CMapCollider::CMapCollider()
{
}

CMapCollider::~CMapCollider()
{
	Release();
}

void CMapCollider::Initialize()
{
	CObject::Initialize();
}

int CMapCollider::Update()
{
	return 0;
}

void CMapCollider::Render(HDC hDC)
{
	CObject::Update_Renderer();
	CObject::Render(hDC);
}

void CMapCollider::Release()
{
}

void CMapCollider::OnCollision(CObject* pObj)
{
}

void CMapCollider::OnCollision_Entity(CObject * pObj)
{
}
