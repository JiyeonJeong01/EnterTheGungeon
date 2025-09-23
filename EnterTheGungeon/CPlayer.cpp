#pragma region INCLUDE
#include "pch.h"
#include "CPlayer.h"

#include "CCameraManager.h"
#include "CInputManager.h"
#include "CObjectFactory.h"
#include "CRelease.h"

#include "CPlayerStateMachine.h"
#include "CState.h"

#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

#include "CPlayerWeapon.h"
#include "CInventory.h"
#include "CReloadBar.h"
#include "CPlayerInfo.h"
#pragma endregion

CPlayer::CPlayer()
{
    pStateMachine = nullptr;
    pCurrentState = nullptr;
    eCurrentState = PS_END;
    fSpeed = 0.f;
    bDodgePlaying = false;
    eDir = D_DOWN;
    pWeapon = nullptr;
    pReloadBar = nullptr;
    pInventory = nullptr;
}

CPlayer::~CPlayer()
{
    // CRelease<CStat*>::Release(pHP);
    Release();
}

void CPlayer::Initialize()
{
    CObject::Initialize();

    pTransform->Position({ WINCX >> 1, WINCY >> 1 });
    pTransform->Size({ 60.f, 60.f });
    pCollider->Size({ 40.f, 60.f });
    pRenderer->Size({ 60.f, 60.f });

    CObject::Update_Collider();
    CObject::Update_Renderer();

    fSpeed = 6.f;
    fLimitFireTime = 0.5f; // TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!! must load gun's info

    pRenderer->rType = RND__GAMEBOJECT;
    eType = O_PLAYER;

    bReloading = false;

    iMaxHP = 6;
    iHP = iMaxHP;

    Initialize_PlayerComponents();
}

int CPlayer::Update()
{
    Handle_Input();
    Update_Renderer();

    pCurrentState->Update();
    pInventory->Update();

    CObject::Update_Collider();
    return 0;
}

void CPlayer::Late_Update()
{
    pCurrentState->Late_Update();
    Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(pTransform->Position());
}

void CPlayer::Render(HDC _hDC)
{
    // Render for debug
    CObject::Render(_hDC);

    pCurrentState->Render(_hDC);
    pInventory->Draw_Inventory(_hDC);
}

void CPlayer::Release()
{
    CRelease<CPlayerStateMachine*>::Release(pStateMachine);
    CRelease<CPlayerWeapon*>::Release(pWeapon);
    CRelease<CInventory*>::Release(pInventory);

}

void CPlayer::Update_Transform()
{
    pTransform->Position().X(pTransform->Position().X() * pTransform->Direction().X() * fSpeed);
    pTransform->Position().Y(pTransform->Position().Y() * pTransform->Direction().Y() * fSpeed);
}

void CPlayer::Update_Renderer()
{
    // CObject::Update_Renderer() computes this based on Transform.position and Transform.size
    CObject::Update_Renderer();
}

void CPlayer::OnCollision(CObject* pObj, Vector2 vDiff)
{
    ObjectType type = pObj->Get_ObjType();
    switch (type)
    {
    case O_ENBULLET:
        OnCollision_EnBullet(pObj, vDiff);
    }







}

void CPlayer::OnCollision_EnBullet(CObject* pObj, Vector2 vDiff)
{
    iHP = (iHP - 1 <= 0 ? 0 : iHP - 1);
}

void CPlayer::Initialize_PlayerComponents()
{
    pStateMachine = new CPlayerStateMachine(this);
    pStateMachine->Initialize();

    pWeapon = new CPlayerWeapon(this);
    pWeapon->Initialize();

    pInventory = new CInventory(this);
    pInventory->Initialize();

    pReloadBar = dynamic_cast<CReloadBar*>(CObjectFactory<CReloadBar>::Create(O_UI));
    pReloadBar->Set_Player(this);

    pPlayerInfo = dynamic_cast<CPlayerInfo*>(CObjectFactory<CPlayerInfo>::Create(O_UI));
    pPlayerInfo->Set_Player(this);

}

void CPlayer::Handle_Input()
{
    vInputDir = {
        MANAGER(CInputManager*, M_INPUT)->Get_Key('A') ? -1.0f : MANAGER(CInputManager*, M_INPUT)->Get_Key('D') ? 1.0f : 0.0f,
        MANAGER(CInputManager*, M_INPUT)->Get_Key('W') ? -1.0f : MANAGER(CInputManager*, M_INPUT)->Get_Key('S') ? 1.0f : 0.0f,
    };

    if (vInputDir.X() == 1.f) // D - Right
    {
        if (vInputDir.Y() == -1.f) eDir = D_UR;
        else if (vInputDir.Y() == 0.f) eDir = D_RIGHT;
        else if (vInputDir.Y() == 1.f) eDir = D_DR;
    }
    else if (vInputDir.X() == -1.f) // A - Left
    {
        if (vInputDir.Y() == -1.f) eDir = D_UL;
        else if (vInputDir.Y() == 0.f) eDir = D_LEFT;
        else if (vInputDir.Y() == 1.f) eDir = D_DL;
    }
    else if (vInputDir.X() == 0.f)
    {
        if (vInputDir.Y() == -1.f) eDir = D_UP;
        else if (vInputDir.Y() == 1.f) eDir = D_DOWN;
    }
    else
    {
        eDir = D_END;
    }

    vInputDir.Normalize();

    pInputCursor = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();

    // Reloading 
    if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('R') && !bReloading)
    {
        pReloadBar->Start_Reload();
        bReloading = true;
    }



    if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_LCONTROL))
    {
        MANAGER(CCameraManager*, M_CAMERA)->Set_CamerMode(CCameraManager::Shake);
        pWeapon->Swap_Weapon();
    }

}

void CPlayer::Finist_Reloading()
{
    bReloading = false;
    pWeapon->Reload();
}
