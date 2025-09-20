#pragma region INCLUDE
#include "pch.h"
#include "CPlayer.h"
#include "CRelease.h"
#include "CPlayerStateMachine.h"
#include "CInputManager.h"
#include "CState.h"
#include "CStat.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CCameraManager.h"
#include "CPlayerWeapon.h"
#include "CReloadBar.h"
#include "CObjectFactory.h"
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

    fSpeed = 6.f;
    fLimitFireTime = 0.5f; // TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!! must load gun's info

    pRenderer->rType = RND__GAMEBOJECT;
    eType = O_PLAYER;

    pStateMachine = new CPlayerStateMachine(this);
    pStateMachine->Initialize();

    pWeapon = new CPlayerWeapon(this);
    pWeapon->Initialize();
    bReloading = false;

    pReloadBar = dynamic_cast<CReloadBar*>(CObjectFactory<CReloadBar>::Create(O_UI));
    pReloadBar->Set_Player(this);

    pPlayerInfo = dynamic_cast<CPlayerInfo*>(CObjectFactory<CPlayerInfo>::Create(O_UI));
    pPlayerInfo->Set_Player(this);
}

int CPlayer::Update()
{
    Handle_Input();
    Update_Renderer();

    pCurrentState->Update();
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
}

void CPlayer::Release()
{
    CRelease<CPlayerStateMachine*>::Release(pStateMachine);
    CRelease<CPlayerWeapon*>::Release(pWeapon);
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

}

void CPlayer::Finist_Reloading()
{
    bReloading = false;
    pWeapon->Reload();
}
