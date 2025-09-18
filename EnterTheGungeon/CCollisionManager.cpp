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
                for_each(dst->Get_Collider()->Get_OnCollision()->begin(),
                    dst->Get_Collider()->Get_OnCollision()->end(),
                    [&](function<void(CObject*, Vector2)> listener) -> void {
                        listener(src, overlapped);
                    });
                for_each(src->Get_Collider()->Get_OnCollision()->begin(),
                    src->Get_Collider()->Get_OnCollision()->end(),
                    [&](function<void(CObject*, Vector2)> listener) -> void {
                        listener(dst, overlapped);
                    });
            }
        }
    }
}

Vector2 CCollisionManager::Get_OverlapRect(CObject* dst, CObject* srcList)
{
    float fDistX = abs(dst->Get_Transform()->Position().X() - dst->Get_Transform()->Position().X());
    float fDistY = abs(dst->Get_Transform()->Position().Y() - dst->Get_Transform()->Position().Y());
    
    float fRadX = (dst->Get_Collider()->Size().X() + dst->Get_Collider()->Size().X()) * 0.5f;
    float fRadY = (dst->Get_Collider()->Size().Y() + dst->Get_Collider()->Size().Y()) * 0.5f;

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

void CCollisionManager::Detect_MapCollision(list<CMap*> groundList, list<CObject*> objList)
{
    // 일단 RECT끼리 판단
    for (auto& obj : objList)
    {
        for (auto& ground : groundList)
        {
            Vector2 objPos = obj->Get_Transform()->Position();
            Vector2 objSize = obj->Get_Collider()->Size();

            float fDistX = abs(objPos.X() - ground->Get_Position().X());
            float fDistY = abs(objPos.Y() - ground->Get_Position().Y());

            float fRadX = objSize.X() * 0.5f +(ground->Get_Position().X() - ground->Get_Collider().left);
            float fRadY = objSize.Y() * 0.5f +(ground->Get_Position().Y() - ground->Get_Collider().top);

            if ((fRadX >= fDistX) && (fRadY >= fDistY))
            {
                Vector2 vDiff = { fRadX - fDistX, fRadY - fDistY };
                if (vDiff.X() > vDiff.Y())
                {
                    if (objPos.Y() < ground->Get_Position().Y()) // obj is above
                    {
                        obj->Get_Transform()->Position({ objPos.X(), objPos.Y() - vDiff.Y() });
                    }
                    else
                    {
                        obj->Get_Transform()->Position({ objPos.X(), objPos.Y() + vDiff.Y() });
                    }
                }
                else
                {
                    if (objPos.X() < ground->Get_Position().X()) // obj is left side
                    {
                        obj->Get_Transform()->Position({ objPos.X() - vDiff.X(), objPos.Y() });
                    }
                    else
                    {
                        obj->Get_Transform()->Position({ objPos.X() + vDiff.X(), objPos.Y() });
                    }
                }
            }
        }
    }
}
