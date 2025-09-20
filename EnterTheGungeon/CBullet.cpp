#include "pch.h"
#include "CBullet.h"
#include "CBmpManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CMapCollider.h"

CBullet::CBullet()
{
    fSpeed = 0.f;
}

CBullet::~CBullet()
{
    Release();
}

void CBullet::Load_Resource()
{
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Bullet01.bmp", L"Bullet01");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Bullet02.bmp", L"Bullet02");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Bullet03.bmp", L"Bullet03");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Bullet04.bmp", L"Bullet04");

    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Boss_Bullet.bmp", L"Boss_Bullet");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Boss_Cheese_Bullet.bmp", L"Boss_Cheese");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Boss_Kunai.bmp", L"Boss_Kunai");
    // MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Boss_Bullet.bmp", L"Bullet04");


    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/BulletEffect01.bmp", L"BulletEffect01");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/BulletEffect02.bmp", L"BulletEffect02");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/BulletEffect03.bmp", L"BulletEffect03");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/BulletEffect04.bmp", L"BulletEffect04");
    MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/BulletEffect05.bmp", L"BulletEffect05");
}

void CBullet::Initialize()
{
    CObject::Initialize();
}

// 쌤 코드의 애니메이션 키 받아오는 부분 그대로 구현 시발!
int CBullet::Update()
{
    CObject::Update_Collider();
    Update_Transform();
    Update_AnimFrame();

    return 0;
}

void CBullet::Late_Update()
{
    CObject::Late_Update(); // update_renderer()
}

void CBullet::Render(HDC _hDC)
{

    CObject::Render(_hDC);

    HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);

    GdiTransparentBlt(_hDC,
        pRenderer->Left(),
        pRenderer->Top(),
        (int)pRenderer->Size().X(),
        (int)pRenderer->Size().Y(),
        hMemDC,
        0, 0, 
        (int)pRenderer->Size().X(),
        (int)pRenderer->Size().Y(),
        RGB(255, 0, 255));
}

void CBullet::Release()
{
}

void CBullet::Update_Transform()
{
    Vector2 curPos = pTransform->Position();
    Vector2 curDir = pTransform->Direction();
    pTransform->Position({ curPos.X() + curDir.X() * fSpeed, curPos.Y() + curDir.Y() * fSpeed });
}

void CBullet::Update_AnimFrame()
{
    if (spriteKey != effetAnimKey)
        return;

    if (effectAnim.dwLastPlayTime + effectAnim.vTransitTime[effectAnim.iCurrIndex] < GetTickCount())
    {
        effectAnim.iCurrIndex++;
        effectAnim.dwLastPlayTime = GetTickCount();

        if (effectAnim.iCurrIndex >= effectAnim.iEndIndex)
        {
            effectAnim.iCurrIndex = 0;
        }
    }
}

void CBullet::Apply_BulletSprite()
{
    switch (eBulletType)
    {
    case CBullet::B01:                        
        bulletSpriteKey = L"Bullet01"; 
        break;
    case CBullet::B02:                        
        bulletSpriteKey = L"Bullet02"; 
        break;
    case CBullet::B03:                        
        bulletSpriteKey = L"Bullet03"; 
        break;
    case CBullet::B04:                        
        bulletSpriteKey = L"Bullet04"; 
        break;
    case CBullet::BossBullet:            
        bulletSpriteKey = L"Boss_Bullet"; 
        break;
    case CBullet::Boss_Cheese:        
        bulletSpriteKey = L"Boss_Cheese"; 
        break;
    case CBullet::Boss_Summon:     
        bulletSpriteKey = L"Boss_Summon";  
        break;
    case CBullet::Boss_Kunai:            
        bulletSpriteKey = L"Boss_Kunai"; 
        break;
    }
    spriteKey = bulletSpriteKey;
}

void CBullet::Apply_EffectAnim()
{
    switch (eEffectType)
    {
    case CBullet::E01: 
        effetAnimKey = L"BulletEffect01";
        effectAnim.Initialize(0, 5, 0);
        break;
    case CBullet::E02:
        effetAnimKey = L"BulletEffect01";
        effectAnim.Initialize(0, 5, 0);
        break;
    case CBullet::E03:
        effetAnimKey = L"BulletEffect01";
        effectAnim.Initialize(0, 6, 0);
        break;
    case CBullet::E04:
        effetAnimKey = L"BulletEffect01";
        effectAnim.Initialize(0, 3, 0);
        break;
    case CBullet::E05:
        effetAnimKey = L"BulletEffect01";
        effectAnim.Initialize(0, 3, 0);
        break;
    }
}

void CBullet::Set_Direction(Vector2 vDir)
{
    pTransform->Direction(move(vDir)); 
}

bool CBullet::Check_InBound()
{
    return true;
}

void CBullet::OnCollision(CObject* pObj)
{
    if (pObj->Get_ObjType() == O_MAP)
    {
        OnCollision_Map();
    }
}

void CBullet::OnCollision_Map()
{
    spriteKey = effetAnimKey;
}
