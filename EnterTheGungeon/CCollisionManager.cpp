#include "pch.h"
#include "CCollisionManager.h"
#include "CObject.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CMap.h"

void CCollisionManager::Detect_RectCollision(list<CObject*> dstList, list<CObject*> srcList)
{
    RECT rcCol{};
    for (auto& dst : dstList)
    {
        for (auto& src : srcList)
        {
            Vector2 overlapped = Get_OverlapRect(dst, src);
            if (overlapped.X() != 0.f && overlapped.Y() != 0.f)
            {
                dst->OnCollision(src, overlapped);
                src->OnCollision(dst, overlapped);
            }
        }
    }
}

Vector2 CCollisionManager::Get_OverlapRect(CObject* dst, CObject* src)
{
    float fDistX = abs(dst->Get_Transform()->Position().X() - src->Get_Transform()->Position().X());
    float fDistY = abs(dst->Get_Transform()->Position().Y() - src->Get_Transform()->Position().Y());
    
    float fRadX = (dst->Get_Collider()->Size().X() + src->Get_Collider()->Size().X()) * 0.5f;
    float fRadY = (dst->Get_Collider()->Size().Y() + src->Get_Collider()->Size().Y()) * 0.5f;

    if ((fRadX >= fDistX) && (fRadY >= fDistY))
    {
        return Vector2{fRadX - fDistX, fRadY - fDistY };
    }
    return Vector2{ 0.f, 0.f };
}

void CCollisionManager::Detect_CircleCollision(list<CObject*> dstList, list<CObject*> srcList)
{
}

Vector2 CCollisionManager::Get_OverlapCircle(CObject* dst, CObject* src)
{
    return Vector2{ 0.f, 0.f };
}

void CCollisionManager::Detect_MapCollision(list<CMap*> mapList, list<CObject*> objList)
{
    for (auto& obj : objList)
    {
        for (auto& map : mapList)
        {          
            Vector2 objPos = obj->Get_Transform()->Position();
            Vector2 objSize = obj->Get_Collider()->Size();

            float fDistX = abs(objPos.X() - map->Get_Transform()->Position().X());
            float fDistY = abs(objPos.Y() - map->Get_Transform()->Position().Y());

            float fRadX = objSize.X() * 0.5f +(map->Get_Transform()->Position().X() - map->Get_Collider()->Left());
            float fRadY = objSize.Y() * 0.5f +(map->Get_Transform()->Position().Y() - map->Get_Collider()->Top());

            if ((fRadX >= fDistX) && (fRadY >= fDistY))
            {
               // 충돌 확정
                Vector2 vDiff = { fRadX - fDistX, fRadY - fDistY };
                 map->OnCollision(obj, vDiff);
             }
        }
    }
}
