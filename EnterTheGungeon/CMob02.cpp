#include "pch.h"
#include "CMob02.h"

#include "CRelease.h"

#include "CMob02StateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

CMob02::CMob02()
{
    pStateMachine = nullptr;
    pCurrentState = nullptr;
}

CMob02::~CMob02()
{
    Release();
}

void CMob02::Initialize()
{
    CMob::Initialize();

    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 100.f, 100.f });
    pCollider->Size({ 100.f, 100.f });
    pRenderer->Size({ 100.f, 100.f });

    pStateMachine = new CMob02StateMachine(this);
    pStateMachine->Initialize();

    fSpeed = 2.3f;

    iMaxHP = 1;
    iHP = iMaxHP;
}

int CMob02::Update()
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

void CMob02::Late_Update()
{
    CObject::Update_Renderer();
    CMob::Check_Invincible();
    pCurrentState->Late_Update();
    Update_Transform();
}

void CMob02::Render(HDC _hDC)
{
    CObject::Render(_hDC);
    pCurrentState->Render(_hDC);
}

void CMob02::Release()
{
    CRelease<CMob02StateMachine*>::Release(pStateMachine);
}

void CMob02::Update_Transform()
{
    Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
                             pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
    pTransform->Position(move(vNew));
}
