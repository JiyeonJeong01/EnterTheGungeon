#include "pch.h"
#include "CMob01.h"
#include "CObjectFactory.h"
#include "CInputManager.h"
#include "CRelease.h"

#include "CMob01StateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CMob01IdleState.h"

CMob01::CMob01()
{
    pStateMachine = nullptr;
    pCurrentState = nullptr;
}

CMob01::~CMob01()
{
    Release();
}

void CMob01::Initialize()
{
    CMob::Initialize();

    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 80.f, 80.f });
    pCollider->Size({ 80.f, 80.f });
    pRenderer->Size({ 80.f, 80.f });

    pStateMachine = new CMob01StateMachine(this);
    pStateMachine->Initialize();
    fSpeed = 1.5f;

}

int CMob01::Update()
{
    CObject::Update_Collider();

    pCurrentState->Update();

    return 0;
}

void CMob01::Late_Update()
{
    CObject::Update_Renderer();
    pCurrentState->Late_Update();
    Update_Transform();
}

void CMob01::Render(HDC _hDC)
{
    CObject::Render(_hDC);
    pCurrentState->Render(_hDC);
}

void CMob01::Release()
{
    CRelease<CMob01StateMachine*>::Release(pStateMachine);
}

void CMob01::Update_Transform()
{
    Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
                             pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
    pTransform->Position(move(vNew));
}
