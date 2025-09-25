#include "pch.h"
#include "CBossSummonAttackState.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CCameraManager.h"
#include "CObjectFactory.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CBossStateMachine.h"
#include "CBossBullet.h"
#include  "CPlayer.h"

void CBossSummonAttackState::Initialize()
{
	CBossState::Initialize();

	eState = CBoss::BS_CHEESEATTACK;
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_SummonAttack.bmp", L"Boss_SummonAttack");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Boss_Cheese_Bomb.bmp", L"Boss_Cheese_Bomb");
    animation.Initialize(0, 11, (int)D_DOWN);

	dwLastFireTime = GetTickCount();
    fWaitTime = 2.5f;
}

void CBossSummonAttackState::Update()
{
	CBossState::Update();
	Update_AnimFrame();

    if (curState == SummonEnd) return;
    if (curState == Creating)
    {
        Create_Cheese();
    }
    else if (curState == ShotWating)
    {
        Wait_Shot();
    }
    else if (curState == Summon)
    {
        Summoning();
    }
    else if (curState == ReshotWating)
    {
        Reshot_Waiting();
    }
    else if (curState == ReShot)
    {
        ReShooting();
    }
}

void CBossSummonAttackState::Late_Update()
{
    Wait_Shot();
}

void CBossSummonAttackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_SummonAttack");

	CRenderer renderer = *(pObj->Get_Renderer());

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)renderer.Size().X(),
		0,
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		RGB(255, 0, 255));

    if (curState == ReshotWating || curState == Summon)
    {
        Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ (float) rBound.left, (float)rBound.top });
        HDC summon = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_Cheese_Bomb");
        GdiTransparentBlt(hDC,
            vRenderPos.X() - 100, vRenderPos.Y() - 100,
            300, 300,
            summon,
            iBombCurrIndex * 300,
            0,
            300, 300,
            RGB(255, 0, 255));
        if (iBombCurrIndex == 6)
        {
            MANAGER(CSoundManager*, M_SOUND)->StopSound(SOUND_EFFECT);
            MANAGER(CSoundManager*, M_SOUND)->PlaySoundW(L"Boss_SummonBomb.wav", SOUND_EFFECT, 1.f);
        }
    }
}

void CBossSummonAttackState::Release()
{
}

void CBossSummonAttackState::Exit()
{
    rBound = {};
    curState = SummonEnd;
    dwWaitElapsedTime = GetTickCount();
    iBombCurrIndex = 0;
    dwReshotWaitingTime = GetTickCount();
    static_cast<CBoss*>(pObj)->Set_LastAttackTime();
    static_cast<CBossStateMachine*>(pStateMachine)->Set_PrevAttack(CBoss::BS_SUMMONATTACK);
}

void CBossSummonAttackState::Enter()
{
    curState = Creating;
    iCurBullet = iTotalBullet = 0;
    iBombCurrIndex = 0;
    iBombCurrIndex = 0;
    rCameraToRealPos = {};
    reshotDirList.clear();
    bulletList.clear();
    curState = Creating;
    rBound = {};
	Vector2 vLT = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ 0, 0 });
	Vector2 vRT = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ WINCX, WINCY });

	rCameraToRealPos = { static_cast<int>(vLT.X()), static_cast<int>(vLT.Y()), static_cast<int>(vRT.X()), static_cast<int>(vRT.Y()) };

    int bulletsPerEdge = 60;

    int left = rCameraToRealPos.left + 30;
    int top = rCameraToRealPos.top + 30;
    int right = rCameraToRealPos.right - 30;
    int bottom = rCameraToRealPos.bottom - 30;
    
    const int offsetRange = 70; 

    // 상단
    for (int i = 0; i < bulletsPerEdge; ++i)
    {
        int randX = rand() % (right - left) + left;
        int randY = top + (rand() % (offsetRange * 2 + 1) - offsetRange); // top ± offsetRange

        CBossBullet* pBullet = static_cast<CBossBullet*>(   CObjectFactory<CBossBullet>::Create(O_ENBULLET, randX, randY)   );
        pBullet->Set_BulletType(CBullet::Boss_Summon);
        pBullet->Set_EffectType(CBullet::E04);
        pBullet->Apply_BulletSprite();
        pBullet->Apply_EffectAnim();
        pBullet->Set_SummonBulletIndex(1);
        bulletList.push_back(pBullet);
        iTotalBullet++;
    }

    // 하단
    for (int i = 0; i < bulletsPerEdge; ++i)
    {
        int randX = rand() % (right - left) + left;
        int randY = bottom + (rand() % (offsetRange * 2 + 1) - offsetRange); // bottom ± offsetRange

        CBossBullet* pBullet = static_cast<CBossBullet*>(  CObjectFactory<CBossBullet>::Create(O_ENBULLET, randX, randY)    );
        pBullet->Set_BulletType(CBullet::Boss_Summon);
        pBullet->Set_EffectType(CBullet::E04);
        pBullet->Apply_BulletSprite();
        pBullet->Apply_EffectAnim();
        pBullet->Set_SummonBulletIndex(0);
        bulletList.push_back(pBullet);
        iTotalBullet++;
    }

    // 좌측
    for (int i = 0; i < bulletsPerEdge; ++i)
    {
        int randX = left + (rand() % (offsetRange * 2 + 1) - offsetRange); // left ± offsetRange
        int randY = rand() % (bottom - top) + top;

        CBossBullet* pBullet = static_cast<CBossBullet*>(     CObjectFactory<CBossBullet>::Create(O_ENBULLET, randX, randY)  );
        pBullet->Set_BulletType(CBullet::Boss_Summon);
        pBullet->Set_EffectType(CBullet::E04);
        pBullet->Apply_BulletSprite();
        pBullet->Apply_EffectAnim();
        pBullet->Set_SummonBulletIndex(2);
        bulletList.push_back(pBullet);
        iTotalBullet++;
    }

    // 우측
    for (int i = 0; i < bulletsPerEdge; ++i)
    {
        int randX = right + (rand() % (offsetRange * 2 + 1) - offsetRange); // right ± offsetRange
        int randY = rand() % (bottom - top) + top;

        CBossBullet* pBullet = static_cast<CBossBullet*>(   CObjectFactory<CBossBullet>::Create(O_ENBULLET, randX, randY) );
        pBullet->Set_BulletType(CBullet::Boss_Summon);
        pBullet->Set_EffectType(CBullet::E04);
        pBullet->Apply_BulletSprite();
        pBullet->Apply_EffectAnim();
        pBullet->Set_SummonBulletIndex(3);
        bulletList.push_back(pBullet);
        iTotalBullet++;
    }

    for (auto& pBullet : bulletList)
    {
        pBullet->bCanRend = false;
    }
}

void CBossSummonAttackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CBossSummonAttackState::Stop_Animation()
{
}

void CBossSummonAttackState::On_End_Animation()
{
}

void CBossSummonAttackState::Create_Cheese()
{
    int spawnCount = rand() % 6 + 1;

    int activated = 0;
    for (auto& pBullet : bulletList)
    {
        if (!pBullet) continue;
        if (pBullet->bCanRend) continue;

        pBullet->bCanRend = true;
        pBullet->Get_Transform()->Direction({ 0.f, 0.f });
        pBullet->rSummoned = false;
        activated++;
        iCurBullet++;
        if (iCurBullet >= iTotalBullet)
        {
            curState = ShotWating;
            dwWaitElapsedTime = GetTickCount();
            iCurBullet = 0;
        }
        if (activated >= spawnCount)
            break;
    }
}

void CBossSummonAttackState::Wait_Shot()
{
    if (dwWaitElapsedTime + fWaitTime * 1000 < GetTickCount() && curState != ReshotWating && curState != ReShot)
    {
        curState = Summon;
        return;
    }


    for (auto& pBullet : bulletList)
    {
        if (!pBullet) continue;

        Vector2 vPos = pBullet->Get_Transform()->Position();
        bool moveHorizontal = rand() % 2 == 0;

        if (moveHorizontal)
        {
            int offset = (rand() % 3) - 1; // -1, 0, 1
            vPos.X(vPos.X() + offset);
        }
        else
        {
            int offset = (rand() % 3) - 1; // -1, 0, 1
            vPos.Y(vPos.Y() + offset);
        }

        pBullet->Get_Transform()->Position(move(vPos));
    }
}


void CBossSummonAttackState::Summoning()
{
    int spawnCount = rand() % 3 + 1;
    int sizeX = 50, sizeY = 50; 
    int summoned = 0;
    for (auto& pBullet : bulletList)
    {
        if (!pBullet) continue;
        if (pBullet->rSummoned) continue;

        CBossState::Detect_Player();

        if (iCurBullet == 0)
        {
            Vector2 vPlayerPos = pPlayer->Get_Transform()->Position();
            Vector2 vBulletPos = pBullet->Get_Transform()->Position();
            Vector2 vDiff = vPlayerPos - vBulletPos;
            Compute_Dir(vDiff);
            vDirToPlayer = vDiff.Get_Normalized();
            reshotDirList.push_back(vDirToPlayer);

            rBound = {
                static_cast<LONG>(vPlayerPos.X() - sizeX), // left
                static_cast<LONG>(vPlayerPos.Y() - sizeY), // top
                static_cast<LONG>(vPlayerPos.X() + sizeX), // right
                static_cast<LONG>(vPlayerPos.Y() + sizeY)  // bottom
            };
        }
        else
        {
            Vector2 vBoundCenter = {
                (rBound.left + rBound.right) * 0.5f,
                (rBound.top + rBound.bottom) * 0.5f
            };

            Vector2 vBulletPos = pBullet->Get_Transform()->Position();
            Vector2 vDiff = vBoundCenter - vBulletPos;
            Compute_Dir(vDiff);
            vDirToPlayer = vDiff.Get_Normalized();
            reshotDirList.push_back(vDirToPlayer);
        }
        pBullet->Set_Direction(vDirToPlayer);
        pBullet->Set_Speed(25.f);
        pBullet->rSummoned = true;
        pBullet->rSummonBound = rBound;

        summoned++;
        iCurBullet++;
        if (iCurBullet >= iTotalBullet)
        {
            curState = ReshotWating;
            dwReshotWaitingTime = GetTickCount();
            
        }
        if (summoned >= spawnCount)
            break;
    }
}

void CBossSummonAttackState::Reshot_Waiting()
{
    if (dwReshotWaitingTime + 80 < GetTickCount())
    {
        dwReshotWaitingTime = GetTickCount();
        iBombCurrIndex++;
        if (iBombCurrIndex >= 13)
        {
            curState = ReShot;
        }
    }
}

void CBossSummonAttackState::ReShooting()
{
    auto dirIter = reshotDirList.begin();
    for (auto bulletIter = bulletList.begin();
        bulletIter != bulletList.end() && dirIter != reshotDirList.end();
        ++bulletIter, ++dirIter)
    {
        CBossBullet* pBullet = *bulletIter;
        if (!pBullet) continue;
        pBullet->bCanRend = true;
        pBullet->Set_Direction(*dirIter * -1.f);

        pBullet->Set_Speed(35.f);
        pBullet->bSummonCanDestoryed = true;
    }
    curState = SummonEnd;
    pStateMachine->Change_State(CBoss::BS_IDLE);
}