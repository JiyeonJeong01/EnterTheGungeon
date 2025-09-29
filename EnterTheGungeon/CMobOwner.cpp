#include "pch.h"
#include "CMobOwner.h"

#include "CBmpManager.h"
#include "CRelease.h"

#include "CMobOwnerStateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

CMobOwner::CMobOwner()
{
    pStateMachine = nullptr;
    pCurrentState = nullptr;
}

CMobOwner::~CMobOwner()
{
    Release();
}

void CMobOwner::Initialize()
{
    CMob::Initialize();

    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Owner.bmp", L"Owner");



    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 200.f, 200.f });
    pCollider->Size({ 160.f, 180.f });
    pRenderer->Size({ 200.f, 200.f });

    pStateMachine = new CMobOwnerStateMachine(this);
    pStateMachine->Initialize();

    fSpeed = 0.01f;

    iMaxHP = 5;
    iHP = iMaxHP;
}

int CMobOwner::Update()
{
    if (bAlive == false) return S_DEAD;

    CObject::Update_Collider();

    if (bKnockback && eCurrentState != CMob::Dead && eCurrentState != CMob::Walk)
    {
        bKnockback = false;
        pStateMachine->Change_State(CMob::Walk);
    }

    if (iHP <= 0 && eCurrentState != CMob::MobState::Dead)
        pStateMachine->Change_State(CMob::Dead);

    pCurrentState->Update();

    return 0;
}

void CMobOwner::Late_Update()
{
    CObject::Update_Renderer();
    CMob::Check_Invincible();
    pCurrentState->Late_Update();
    Update_Transform();
}

void CMobOwner::Render(HDC _hDC)
{
    CObject::Render(_hDC);
    pCurrentState->Render(_hDC);
}

void CMobOwner::Release()
{
    CRelease<CMobOwnerStateMachine*>::Release(pStateMachine);
}

void CMobOwner::Update_Transform()
{

}
