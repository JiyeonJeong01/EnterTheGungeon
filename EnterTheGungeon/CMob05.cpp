#include "pch.h"
#include "CMob05.h"

#include "CRelease.h"

#include "CMob05StateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

CMob05::CMob05()
{
    pStateMachine = nullptr;
    pCurrentState = nullptr;
}

CMob05::~CMob05()
{
    Release();
}

void CMob05::Initialize()
{
    CMob::Initialize();

    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 110.f, 110.f });
    pCollider->Size({ 60.f, 80.f });
    pRenderer->Size({ 110.f, 110.f });

    pStateMachine = new CMob05StateMachine(this);
    pStateMachine->Initialize();

    fSpeed = 1.5f;

    iMaxHP = 5;
    iHP = iMaxHP;
}

int CMob05::Update()
{
    if (bAlive == false) return S_DEAD;
    CObject::Update_Collider();
    if (bKnockback && eCurrentState != CMob::Dead)
    {
        bKnockback = false;
        pStateMachine->Change_State(CMob::Damaged);
    }

    if (iHP <= 0 && eCurrentState != CMob::MobState::Dead)
        pStateMachine->Change_State(CMob::Dead);

    pCurrentState->Update();

    return 0;
}

void CMob05::Late_Update()
{
    CObject::Update_Renderer();
    CMob::Check_Invincible();
    pCurrentState->Late_Update();
    Update_Transform();
}

void CMob05::Render(HDC _hDC)
{
    CObject::Render(_hDC);
    pCurrentState->Render(_hDC);
}

void CMob05::Release()
{
    CRelease<CMob05StateMachine*>::Release(pStateMachine);
}

void CMob05::Update_Transform()
{
    Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
                             pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
    pTransform->Position(move(vNew));
}
