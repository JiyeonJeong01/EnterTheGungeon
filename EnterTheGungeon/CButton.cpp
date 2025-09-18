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

    CObject::Update_Renderer();
    CObject::Update_Collider();
}

void CButton::Render(HDC hDC)
{
    if (!bActive) return;
    Rectangle(hDC, pRenderer->Left(), pRenderer->Top(), pRenderer->Right(), pRenderer->Bottom());
}

void CButton::Release()
{
    CRelease<CPointerHandler*>::Release(pPointer);
}
