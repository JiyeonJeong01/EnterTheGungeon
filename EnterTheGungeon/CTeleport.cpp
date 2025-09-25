#include "pch.h"
#include "CTeleport.h"
#include "CObjectManager.h"
#include "CInputManager.h"
#include "CBmpManager.h"
#include "CSceneManager.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"

#include "CPlayer.h"

CTeleport::CTeleport()
{
	pPlayer = nullptr;
	animKey = nullptr;
}

CTeleport::~CTeleport()
{
	Release();

}

void CTeleport::Initialize()
{
	CMap::Initialize();

	pTransform->Size({ 350, 350 });
	pCollider->Size({ 350, 350 });
	pRenderer->Size({ 350, 350 });

	eType = O_INTERACTABLE;
	pRenderer->rType = RND__GAMEBOJECT;

	pPlayer = dynamic_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
	if (pPlayer == nullptr) printf("error! - can't find player!\n");

	iAnimCol = 0;

	animKey = L"Teleport_Active";  //Teleport_Active or Teleport_Opened

	dwAnimElapsedTime = GetTickCount();
	fAnimTransit = 0.2;;
}

int CTeleport::Update()
{

	return 0;
}

void CTeleport::Late_Update()
{
	if (dwAnimElapsedTime + fAnimTransit * 1000 < GetTickCount())
	{
		dwAnimElapsedTime = GetTickCount();
		iAnimCol++;
		if (iAnimCol > 3)
		{
			iAnimCol = 0;
		}

	}
}

void CTeleport::Render(HDC hDC)
{
	CMap::Render(hDC);

	HDC tableDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Teleport_Opened");

	GdiTransparentBlt(hDC,
		pRenderer->Left(),
		pRenderer->Top(),
		(int)pRenderer->Size().X(),
		(int)pRenderer->Size().Y(),
		tableDC,
		iAnimCol * pRenderer->Size().X(), 0,
		(int)pRenderer->Size().X(),
		(int)pRenderer->Size().Y(),
		RGB(55, 55, 55));
}

void CTeleport::Release()
{
}

void CTeleport::Update_Collider()
{
	CObject::Update_Collider();
}

void CTeleport::OnCollision(CObject* pObj, Vector2 vDiff)
{
	ObjectType oType = pObj->Get_ObjType();

	switch (oType)
	{
	case O_PLAYER:
		OnCollision_Entity(pObj, vDiff);
		break;
	}
}

void CTeleport::OnCollision_Entity(CObject* pObj, Vector2 vDiff)
{
	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RETURN))
		MANAGER(CSceneManager*, M_SCENE)->Change_Scene(
			(SceneType)(MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene()+1)
		);
}
