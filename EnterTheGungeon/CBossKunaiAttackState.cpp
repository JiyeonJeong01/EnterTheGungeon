#pragma region INCLUDE
#include "pch.h"
#include "CBossKunaiAttackState.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CCameraManager.h"
#include "CMapManager.h"
#include "CObjectFactory.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CBossStateMachine.h"
#include "CBossBullet.h"
#include  "CPlayer.h"
#include "CMap.h"
#pragma endregion

using namespace Gdiplus;

void CBossKunaiAttackState::Initialize()
{
    CBossState::Initialize();

    eState = CBoss::BS_KUNAIATTACK;
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_KunaiAttack.bmp", L"Boss_KunaiAttack");

    animation.Initialize(0, 8, (int)D_DOWN);
}

void CBossKunaiAttackState::Update()
{
    CBossState::Update();
    Update_AnimFrame();

    if (curState == KunaiEnd) return;

    if (curState == Setup)
    {
        SettingUp();
    }
    else if (curState == Dodge)
    {
        Dodging();
    }
    else if (curState == Shot)
    {
        Shooting();
    }
    else if (curState == Wait)
    {
        Waiting();
    }

    //printf("current boss kunai state : %d\n", curState);
}

void CBossKunaiAttackState::Late_Update()
{
    Waiting();
}

void CBossKunaiAttackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(animKey);

	CRenderer renderer = *(pObj->Get_Renderer());

    if (bDrawLine)
    {
        Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(pObj->Get_Transform()->Position());
        POINT bossPos = { (int)vRenderPos.X(), (int)vRenderPos.Y() };

        HPEN hPen = CreatePen(PS_DOT, 5, RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

        for (int i = 0; i < iShotDirCount; ++i)
        {
            MoveToEx(hDC, bossPos.x, bossPos.y, NULL);
            vShotDir[i].Normalize();
            LineTo(hDC, (int)(bossPos.x + vShotDir[i].X() * (fCurDistToPlayer + i * 100)), 
                (int)(bossPos.y + vShotDir[i].Y() * (fCurDistToPlayer + i * 100)));
        }
        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
    }

    // Boss
    int animRow = 0;
    if (curState == Dodge || curState == Setup)
    {
        animRow = iDodgeAnimRow;
    }

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)renderer.Size().X(),
        animRow * (int)renderer.Size().Y(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		RGB(255, 0, 255));
}

void CBossKunaiAttackState::Release()
{
}

void CBossKunaiAttackState::Exit()
{
    static_cast<CBoss*>(pObj)->Set_Speed(3.f);
    static_cast<CBossStateMachine*>(pStateMachine)->Set_PrevAttack(CBoss::BS_KUNAIATTACK);
}

void CBossKunaiAttackState::Enter()
{
    curState = Setup;
    iAttackCount = 0;
    iDodgeAnimCol = 0;
    bDrawLine = false;
    fill(begin(bDodged), end(bDodged), false);
    animKey = L"Boss_DODGE";
    vDodgeDestPos = vSetupPos;
    Compute_DodgeDir();
    Compute_DodgeAnimRow(eDir);
    static_cast<CBoss*>(pObj)->Set_Speed(8.f); // original speed is 3.f

    bDodgeTimer = false;
}

void CBossKunaiAttackState::Update_AnimFrame()
{
    CState::Update_AnimFrame();
}

void CBossKunaiAttackState::Stop_Animation()
{
}

void CBossKunaiAttackState::On_End_Animation()
{
    if (iAttackCount < iTotalAttack && curState == Shot && bShoot)
    {
        bShoot = false;
        curState = Dodge;
        animKey = L"Boss_DODGE";
        Before_Dodge();
        bDodgeTimer = true;
    }
}

void CBossKunaiAttackState::SettingUp()
{
    Vector2 vCurPos = pObj->Get_Transform()->Position();
    vDirToDest = { vDodgeDestPos.X() - vCurPos.X(),  vDodgeDestPos.Y() - vCurPos.Y() };

    if (vDirToDest.Get_Magnitude() <= 5.f)
    {
        // 도착
        pObj->Get_Transform()->Direction({ 0.f, 0.f });
        curState = Shot;
        animKey = L"Boss_KunaiAttack";
    }
    else
    {
        // 이동
        vDirToDest.Normalize();
        pObj->Get_Transform()->Direction(move(vDirToDest));
    }
    Dodging();
}

void CBossKunaiAttackState::Before_Dodge()
{
    Vector2 vDir[8] = { { 0.f, -1.f }, { 0.f, 1.f }, { -1.f, 0.f  }, { 1.f, 0.f }, { -1.f, -1.f }, { 1.f, -1.f }, {1.f, -1.f}, {1.f, 1.f} };
    int iRand = rand() % 8;
    
    while (bDodged[iRand])
    {
        iRand = rand() % 8;
    }
    bDodged[iRand] = true;
    pObj->Get_Transform()->Direction(move(vDir[iRand]));

    animKey = L"Boss_DODGE";
    Compute_DodgeAnimRow((Direction)iRand);
    iDodgeAnimCol = 0;
}

void CBossKunaiAttackState::Dodging()
{
    // Play Animaition
    if (dwDodgeAnimTime + 80 < GetTickCount())
    {
        dwDodgeAnimTime = GetTickCount();
        iDodgeAnimCol++;
        if (iDodgeAnimCol > iDodgeAnimTotalFrame)
        {
            iDodgeAnimCol = 0;
        }
    }

    if (bDodgeTimer && iDodgeAnimCol == 0)
    {
        curState = Shot;
        animKey = L"Boss_KunaiAttack";
        animation.iCurrIndex = 0;
    }
}

void CBossKunaiAttackState::Shooting()
{
    if (animation.iCurrIndex <= 6)
    {
        bDrawLine = true;
        bCanShot = true;
        bShoot = false;
        // 1. theta 구하기
        float fRad = atan2f(vDirToPlayer.Y(),  vDirToPlayer.X());
        float fDegree = fRad * 180.f / PI;

        //2. 발사 각도
        float fDist = vDirToPlayer.Get_Magnitude();
        vShotDir[0] = vDirToPlayer;
        vShotDir[1] = { cosf((fDegree - iDegreeRange) * PI / 180.f) * fDist, sinf((fDegree - iDegreeRange) * PI / 180.f) };
        vShotDir[2] = { cosf((fDegree + iDegreeRange) * PI / 180.f) * fDist, sinf((fDegree + iDegreeRange) * PI / 180.f) };
       
        if (animation.iCurrIndex >=1 && animation.iCurrIndex <= 3)
        {
            // 궤도
            bDrawLine = (animation.iCurrIndex % 2 == 0);
        }
    }
    else if (animation.iCurrIndex == 7 && bCanShot)
    {
        bDrawLine = false;
        for (int i = 0; i < iShotDirCount; ++i)
        {
            CBossBullet* pBullet = static_cast<CBossBullet*>(CObjectFactory<CBossBullet>::Create(
                O_ENBULLET, pObj->Get_Transform()->Position().X(), pObj->Get_Transform()->Position().Y()));
            pBullet->Set_Direction({ vShotDir[i] });
            pBullet->Set_BulletType(CBullet::Boss_Kunai);
            pBullet->Set_EffectType(CBullet::E04);
            pBullet->Apply_BulletSprite();
            pBullet->Apply_EffectAnim();
            pBullet->Set_Speed(40.f);
        }
        bCanShot = false;
        bShoot = true;
        iAttackCount++;
        if (iAttackCount >= iTotalAttack)
        {
            pStateMachine->Change_State(CBoss::BS_IDLE);
        }
    }
}

void CBossKunaiAttackState::Waiting()
{
}

void CBossKunaiAttackState::Compute_DodgeDir()
{
    Vector2 vCurPos = pObj->Get_Transform()->Position();
    Vector2 vDiff = vDodgeDestPos -vCurPos;

    float fAlpha = 150.f;
    if (vDiff.X() > 0) // 목표 지점이 보스의 오른쪽
    {
        if (vDiff.Y() < 0)
        {
            if (vDiff.X() < fAlpha) eDir = D_UP;
            else 	eDir = D_UR;
        }
        else
        {
            if (vDiff.X() < fAlpha) 	eDir = D_DOWN;
            else 	eDir = D_DR;
        }
    }
    else
    {
        if (vDiff.Y() < 0)
        {
            if (vDiff.X() > -fAlpha)  	eDir = D_UP;
            else  	eDir = D_UL;
        }
        else
        {
            if (vDiff.X() > -fAlpha) 	eDir = D_DOWN;
            else 		eDir = D_DL;
        }
    }
}

void CBossKunaiAttackState::Compute_DodgeAnimRow(Direction eDir)
{
    switch (eDir)
    {
    case D_UP: // 위로  
        iDodgeAnimRow = 3;
        break;
    case D_DOWN: // 아래로
        iDodgeAnimRow = 0;
        break;
    case D_LEFT: case D_DL:
        iDodgeAnimRow = 1;
        break;
    case D_RIGHT: case D_DR:
        iDodgeAnimRow = 2;
        break;
    case D_UL:
        iDodgeAnimRow = 4;
        break;
    case D_UR:
        iDodgeAnimRow = 5;
        break;
    default:
        iDodgeAnimRow = 0;
        break;
    }
}