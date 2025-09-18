#pragma region INCLUDE
#include "pch.h"
#include "CObject.h"
#include "CManager.h"

#include "CRelease.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"
#pragma endregion

CObject::CObject()
{
	pTransform = nullptr;
	pCollider = nullptr;
	pRenderer = nullptr;
	bAlive = true;
}

CObject::~CObject()
{
	CRelease<CTransform*>::Release(pTransform);
	CRelease<CCollider*>::Release(pCollider);
	CRelease<CRenderer*>::Release(pRenderer);
}

void CObject::Initialize()
{
	pTransform = new CTransform(this);
	pCollider = new CCollider(this);
	pRenderer = new CRenderer(this);
}

void CObject::Late_Update()
{
	Update_Renderer();
}

void CObject::Update_Renderer()
{
	Vector2 vPosition = pTransform->Position();
	Vector2 vSize = pRenderer->Size();

	pRenderer->Left(vPosition.X() - vSize.X() / 2.f);
	pRenderer->Right(vPosition.X() + vSize.X() / 2.f);
	pRenderer->Top(vPosition.Y() - vSize.Y() / 2.f);
	pRenderer->Bottom(vPosition.Y() + vSize.Y() / 2.f);
}

void CObject::Update_Collider()
{
	Vector2 vPosition = pTransform->Position();
	Vector2 vSize = pCollider->Size();

	pCollider->Left(vPosition.X() - vSize.X() / 2.f);
	pCollider->Right(vPosition.X() + vSize.X() / 2.f);
	pCollider->Top(vPosition.Y() - vSize.Y() / 2.f);
	pCollider->Bottom(vPosition.Y() + vSize.Y() / 2.f);
}
