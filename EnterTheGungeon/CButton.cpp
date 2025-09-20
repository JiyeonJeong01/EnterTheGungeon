#pragma region INCLUDE
#include "pch.h"
#include "CButton.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"

#include "CRelease.h"
#pragma endregion

CButton::CButton()
{
    pPointer = new CPointerHandler(this);
    bActive = true;;
}

CButton::~CButton()
{
    Release();
}

void CButton::Initialize()
{
    CObject::Initialize();
    pRenderer->rType = RND__UI;
    pPointer->Initialize();
}

int CButton::Update()
{
    if (!bActive) return 0;
    pPointer->Update_PointerState();

    if (pPointer->Add_OnBeginClick())
        for (auto& event : onBeginClick)
            event();

    if (pPointer->Add_OnBeginHover())
        for (auto& event : onBeginHover)
            event();

    return 0;
}

void CButton::Late_Update()
{
    if (!bActive) return;
    CObject::Update_Collider();
    Update_Renderer();
}

void CButton::Render(HDC hDC)
{
    if (!bActive) return;
    //printf("%d ,%d, %d, %d\n", pRenderer->Left(), pRenderer->Top(), pRenderer->Right(), pRenderer->Bottom());
    Rectangle(hDC, pRenderer->Left(), pRenderer->Top(), pRenderer->Right(), pRenderer->Bottom());
}

void CButton::Release()
{
    CRelease<CTransform*>::Release(pTransform);
    CRelease<CCollider*>::Release(pCollider);
    CRelease<CRenderer*>::Release(pRenderer);
    CRelease<CPointerHandler*>::Release(pPointer);
}

void CButton::Update_Renderer()
{
    Vector2 vPosition = pTransform->Position();
    Vector2 vSize = pRenderer->Size();

    pRenderer->Left(vPosition.X() - vSize.X() / 2.f);
    pRenderer->Right(vPosition.X() + vSize.X() / 2.f);
    pRenderer->Top(vPosition.Y() - vSize.Y() / 2.f);
    pRenderer->Bottom(vPosition.Y() + vSize.Y() / 2.f);

   // printf("%d ,%d, %d, %d\n", pRenderer->Left(), pRenderer->Top(), pRenderer->Right(), pRenderer->Bottom());

}
