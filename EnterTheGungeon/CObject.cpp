#pragma region INCLUDE
#include "pch.h"
#include "CObject.h"
#include "CManager.h"

#include "CRelease.h"
#include "CCameraManager.h"

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

/// <summary>
/// All classes derived from CObject must call this method
/// </summary>
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

/// <summary>
/// Draw bounds of transform, collider, renderer
/// </summary>
/// <param name="_hDC"></param>
void CObject::Render(HDC _hDC)
{
#pragma region Debug
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(HOLLOW_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	Vector2 renderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(pTransform->Position());

	Rectangle(_hDC, pTransform->Position().X() - pTransform->Size().X(),
		pTransform->Position().Y() - pTransform->Size().Y(),
		pTransform->Position().X() + pTransform->Size().X(),
		pTransform->Position().Y() - pTransform->Size().Y());
	Rectangle(_hDC, pRenderer->Left(), pRenderer->Top(), pRenderer->Right(), pRenderer->Bottom());

	Vector2 colliderLT = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ (float)pCollider->Left(), (float)pCollider->Top() });
	Vector2 colliderRB = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ (float)pCollider->Right(), (float)pCollider->Bottom() });
	Rectangle(_hDC, (int)colliderLT.X(), (int)colliderLT.Y(), (int)colliderRB.X(), (int)colliderRB.Y());

	SelectObject(_hDC, hOldBrush);
	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
#pragma endregion
}

void CObject::Update_Transform()
{

}

/// <summary>
/// Update render position on the screen based on real transform.position
/// </summary>
void CObject::Update_Renderer()
{
	Vector2 vPosition = pTransform->Position();
	Vector2 vSize = pRenderer->Size();
	Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(vPosition);

	pRenderer->Left(vRenderPos.X() - vSize.X() / 2.f);
	pRenderer->Right(vRenderPos.X() + vSize.X() / 2.f);
	pRenderer->Top(vRenderPos.Y() - vSize.Y() / 2.f);
	pRenderer->Bottom(vRenderPos.Y() + vSize.Y() / 2.f);

	}

/// <summary>
/// Update collider bounds based on transform.position
/// </summary>
void CObject::Update_Collider()
{
	Vector2 vPosition = pTransform->Position();
	Vector2 vSize = pCollider->Size();

	pCollider->Left(vPosition.X() - vSize.X() / 2.f);
	pCollider->Right(vPosition.X() + vSize.X() / 2.f);
	pCollider->Top(vPosition.Y() - vSize.Y() / 2.f);
	pCollider->Bottom(vPosition.Y() + vSize.Y() / 2.f);
}
