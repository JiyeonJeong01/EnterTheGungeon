#include "pch.h"
#include "CMapGround.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CBullet.h"

CMapGround::CMapGround()
{
}

CMapGround::~CMapGround()
{
    Release();
}

void CMapGround::Initialize()
{
    CMap::Initialize();
    eType = O_MAP;
}

int CMapGround::Update()
{
    return 0;
}

void CMapGround::Render(HDC hDC)
{
    CMap::Render(hDC);
}

void CMapGround::Release()
{
}

void CMapGround::OnCollision(CObject* pObj, Vector2 vDiff)
{
    ObjectType pType = pObj->Get_ObjType();
    if (pType == O_PLAYER || pType == O_ENEMY)
    {
        OnCollision_Entity(pObj, vDiff);
    }
    else if (pType == O_PLBULLET || pType == O_ENBULLET)
    {
        static_cast<CBullet*>(pObj)->OnCollision_MapGround();
    }
}

void CMapGround::OnCollision_Entity(CObject* pObj, Vector2 vDiff)
{
    if ((vDiff.X() != 0.f && vDiff.Y() != 0.f) && pObj != nullptr)
    {
        // 충돌 확정
        if (vDiff.X() > vDiff.Y())
        {
            if (pObj->Get_Transform()->Position().Y() < pTransform->Position().Y()) // obj is above
            {
                pObj->Get_Transform()->Position({ pObj->Get_Transform()->Position().X(), pObj->Get_Transform()->Position().Y() - vDiff.Y() });
            }
            else
            {
                pObj->Get_Transform()->Position({ pObj->Get_Transform()->Position().X(), pObj->Get_Transform()->Position().Y() + vDiff.Y() });
            }
        }
        else
        {
            if (pObj->Get_Transform()->Position().X() < pTransform->Position().X()) // obj is left side
            {
                pObj->Get_Transform()->Position({ pObj->Get_Transform()->Position().X() - vDiff.X(), pObj->Get_Transform()->Position().Y() });
            }
            else
            {
                pObj->Get_Transform()->Position({ pObj->Get_Transform()->Position().X() + vDiff.X(), pObj->Get_Transform()->Position().Y() });
            }
        }
    }
}