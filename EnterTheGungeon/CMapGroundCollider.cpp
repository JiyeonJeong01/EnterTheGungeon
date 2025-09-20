#include "pch.h"
#include "CMapGroundCollider.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CBullet.h"


CMapGroundCollider::CMapGroundCollider()
{
}

CMapGroundCollider::~CMapGroundCollider()
{
    Release();
}

void CMapGroundCollider::Initialize()
{
    CMapCollider::Initialize();
}

int CMapGroundCollider::Update()
{
    return 0;
}

void CMapGroundCollider::Render(HDC hDC)
{
    CMapCollider::Render(hDC);
}

void CMapGroundCollider::Release()
{
}
void CMapGroundCollider::OnCollision(CObject* pObj)
{
    ObjectType pType = pObj->Get_ObjType();
    if (pType == O_PLAYER)
    {
        OnCollision_Entity(pObj);
    }
    else if (pType == O_PLBULLET)
    {
        static_cast<CBullet*>(pObj)->OnCollision_MapGround();
    }
}

void CMapGroundCollider::OnCollision_Entity(CObject* pObj)
{
    Vector2 objPos = pObj->Get_Transform()->Position();
    Vector2 objSize = pObj->Get_Collider()->Size();

    float fDistX = abs(objPos.X() - pTransform->Position().X());
    float fDistY = abs(objPos.Y() - pTransform->Position().Y());

    float fRadX = objSize.X() * 0.5f + (pTransform->Position().X() - pCollider->Left());
    float fRadY = objSize.Y() * 0.5f + (pTransform->Position().Y() - pCollider->Top());

    if ((fRadX >= fDistX) && (fRadY >= fDistY))
    {
        // 충돌 확정
        Vector2 vDiff = { fRadX - fDistX, fRadY - fDistY };
        if (vDiff.X() > vDiff.Y())
        {
            if (objPos.Y() < pTransform->Position().Y()) // obj is above
            {
                pObj->Get_Transform()->Position({ objPos.X(), objPos.Y() - vDiff.Y() });
            }
            else
            {
                pObj->Get_Transform()->Position({ objPos.X(), objPos.Y() + vDiff.Y() });
            }
        }
        else
        {
            if (objPos.X() < pTransform->Position().X()) // obj is left side
            {
                pObj->Get_Transform()->Position({ objPos.X() - vDiff.X(), objPos.Y() });
            }
            else
            {
                pObj->Get_Transform()->Position({ objPos.X() + vDiff.X(), objPos.Y() });
            }
        }
    }
}