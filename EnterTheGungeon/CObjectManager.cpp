#include "pch.h"
#include "CObjectManager.h"
#include "CObject.h"
#include "CRelease.h"
#include "CTransform.h"
#include "CRenderer.h"

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
    Release();
}

void CObjectManager::Initialize()
{

}

void CObjectManager::Update()
{
    for (UINT i = 0; i < OBJECTEND; ++i)
    {
        for (auto iter = m_pObjLists[i].begin(); iter != m_pObjLists[i].end(); )
        {
            int iResult = (*iter)->Update();

            if (iResult == S_DEAD)
            {
                CRelease<CObject*>::Release(*iter);
                iter = m_pObjLists[i].erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }
}

void CObjectManager::Late_Update()
{
    for (UINT i = 0; i < OBJECTEND; ++i)
    {
        for (auto& pObj : m_pObjLists[i])
        {
            pObj->Late_Update();
            if (m_pObjLists[i].empty()) break;

            RenderType rType = pObj->Get_Renderer()->rType;
            m_pRenderList[rType].push_back(pObj);
        }

    }
}

void CObjectManager::Render(HDC hDC)
{
    for (UINT i = 0; i < RND_END; ++i)
    {
        m_pRenderList[i].sort([] (CObject* pDst, CObject* pSrc) -> bool
            {
                return pDst->Get_Transform()->Position().Y() < pSrc->Get_Transform()->Position().Y();
            });
        for (auto& r : m_pRenderList[i])
        {
            r->Render(hDC);
        }
        m_pRenderList[i].clear();
    }
}

void CObjectManager::Release()
{
    for (UINT i = 0; i < OBJECTEND; ++i)
    {
        for_each(m_pObjLists[i].begin(), m_pObjLists[i].end(),
            [&](CObject* pObj) {CRelease<CObject*>::Release(pObj); });
        m_pObjLists[i].clear();
    }
}

bool CObjectManager::Add_Object(ObjectType eObject, CObject* pObj)
{
    if (eObject < 0 || eObject >= OBJECTEND || pObj == nullptr) return false;

    m_pObjLists[eObject].push_back(pObj);

    return true;
}

bool CObjectManager::Remove_Object(ObjectType eObject, CObject* pObj)
{
    if (eObject < 0 || eObject >= OBJECTEND || pObj == nullptr) return false;

    m_pObjLists[eObject].remove(pObj);

    return true;
}

list<CObject*>* CObjectManager::Get_Object(ObjectType eObject)
{
    return &m_pObjLists[eObject];
}

bool CObjectManager::Remov_ObjectAll(ObjectType eObject)
{
    for (auto& pObj : m_pObjLists[eObject])
    {
        CRelease<CObject*>::Release(pObj);
    }
    m_pObjLists[eObject].clear();
    return false;
}
