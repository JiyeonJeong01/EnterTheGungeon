#pragma region INCLUDE
#include "pch.h"
#include "CBoss.h"
#include "CObjectFactory.h"
#include "CInputManager.h"
#include "CRelease.h"

#include "CBossStateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#pragma endregion


CBoss::CBoss()
{
	pStateMachine = nullptr;
	pCurrentState = nullptr;
	eCurrentState = BS_END;
	fSpeed = 0.f;
	bDodgePlaying = false;

	iMaxHP = 0;
	iHP = iMaxHP;
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize()
{
	CObject::Initialize();

    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 100.f, 100.f });
    pCollider->Size({ 100.f, 100.f });
    pRenderer->Size({ 200.f, 200.f });

    CObject::Update_Collider();
    CObject::Update_Renderer();

    fSpeed = 3.f;
    fLimitFireTime = 0.2f;

    pRenderer->rType = RND__GAMEBOJECT;
    eType = O_ENEMY;

    iMaxHP = 100;
    iHP = iMaxHP;
    fAttackTimeRange = 5.f;
    bShouldAttack = false;
    dwLastAttackTime = GetTickCount();
    Initialize_BossComponents();

    MANAGER(CUIManager*, M_UI)->bBossDraw = true;
    MANAGER(CUIManager*, M_UI)->pBoss = this;
}

int CBoss::Update()
{
    Check_ShouldAtack();
    pCurrentState->Update();


    if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('M'))
    {
        iHP--;
    }
    if (iHP <= 0)
    {
        pStateMachine->Change_State(BS_DEAD);
    }

	return 0;
}

void CBoss::Late_Update()
{
    pCurrentState->Late_Update();
    Update_Transform();
    CObject::Update_Collider();
    Update_Renderer();
}

void CBoss::Render(HDC _hDC)
{
    CObject::Render(_hDC);

    pCurrentState->Render(_hDC);

}

void CBoss::Release()
{
    CRelease<CBossStateMachine*>::Release(pStateMachine);
}

void CBoss::Update_Transform()
{
    Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
                                 pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
    pTransform->Position(move(vNew));
}

void CBoss::Update_Renderer()
{
    CObject::Update_Renderer();
}

void CBoss::OnCollision(CObject* pObj, Vector2 vDiff)
{
    ObjectType type = pObj->Get_ObjType();
    if (type == O_PLBULLET)
    {
        this;
        iHP = (iHP - 1 <= 0 ? 0 : iHP - 1);
        printf("boss hp : %d\n", iHP);
    }
}

void CBoss::Initialize_BossComponents()
{
    pStateMachine = new CBossStateMachine(this);
    pStateMachine->Initialize();
}


void CBoss::Check_ShouldAtack()
{
    if (dwLastAttackTime + fAttackTimeRange * 1000 < GetTickCount())
    {
        bShouldAttack = true;
    }
    else
    {
        bShouldAttack = false;
    }
   // printf("boss should attack : %d\n", bShouldAttack);
}