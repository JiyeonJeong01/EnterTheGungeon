#include "pch.h"
#include "CMob03.h"

#include "CRelease.h"

#include "CMob03StateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

CMob03::CMob03()
{
    pStateMachine = nullptr;
    pCurrentState = nullptr;
}

CMob03::~CMob03()
{
    Release();
}

void CMob03::Initialize()
{
    CMob::Initialize();

    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 230.f, 230.f });
    pCollider->Size({ 200.f, 200.f });
    pRenderer->Size({ 230.f, 230.f });

    pStateMachine = new CMob03StateMachine(this);
    pStateMachine->Initialize();

    fSpeed = 0.1f;

    iMaxHP = 1;
    iHP = iMaxHP;
}

int CMob03::Update()
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

void CMob03::Late_Update()
{
    CObject::Update_Renderer();
    CMob::Check_Invincible();
    pCurrentState->Late_Update();
    Update_Transform();
}

void CMob03::Render(HDC _hDC)
{
    CObject::Render(_hDC);
    pCurrentState->Render(_hDC);
}

void CMob03::Release()
{
    CRelease<CMob03StateMachine*>::Release(pStateMachine);
}

void CMob03::Update_Transform()
{
    Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
                             pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
    pTransform->Position(move(vNew));
}
