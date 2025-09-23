#include "pch.h"
#include "CTableObject.h"

#include "CBmpManager.h"
#include "CObjectManager.h"
#include "CInputManager.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"

#include "CPlayer.h"
#include "CBullet.h"

CTableObject::CTableObject()
{
	pPlayer = nullptr;
	animKey = nullptr;
}

CTableObject::~CTableObject()
{
	Release();
}

void CTableObject::Initialize()
{
	CMap::Initialize();

	pTransform->Size({ 100, 100 });
	pCollider->Size({ 90, 65 });
	pRenderer->Size({ 100, 100 });

	eType = O_INTERACTABLE;
	pRenderer->rType = RND__GAMEBOJECT;

	pPlayer = dynamic_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
	if (pPlayer == nullptr) printf("error! - can't find player!\n");
	iDetectSize = 70;

	eState = TableState::Idle;
	
	iAnimRow = 0;
	iAnimCol = 0;
	iAnimEndCol = 3;

	iHitCount = 0;

	animKey = L"Table_IDLE";
	iFlippingFrameTime = 70;

	bCanMove = false;
	bNoDamage = true;
	rColliderOffset = {};
	pCollisionBullet = nullptr;
	bInvincible = false;
}

int CTableObject::Update()
{
	rDetectBound = { (int)pTransform->Position().X() - iDetectSize, (int)pTransform->Position().Y() - iDetectSize,
								(int)pTransform->Position().X() + iDetectSize, (int)pTransform->Position().Y() + iDetectSize, };

	switch (eState)
	{
	case CTableObject::Idle:
		Table_Idle();
		break;
	case CTableObject::Interact:
		Table_Interact();
		break;
	case CTableObject::Flipping:
		Table_Flipping();
		break;
	case CTableObject::Flipped:
		Table_Flipped();
		break;
	case CTableObject::Broken:
		Table_Broken();
		break;
	default:
		break;
	}

	if (dwInvicibleTime + 500 < GetTickCount())
		bInvincible = false;

	return 0;
}

void CTableObject::Late_Update()
{
	Update_Collider();
}

void CTableObject::Render(HDC hDC)
{
	CMap::Render(hDC);

	HDC tableDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(animKey);

	GdiTransparentBlt(hDC,
		pRenderer->Left(),
		pRenderer->Top(),
		(int)pRenderer->Size().X(),
		(int)pRenderer->Size().Y(),
		tableDC,
		iAnimCol * pRenderer->Size().X(),
		iAnimRow * pRenderer->Size().Y(),
		(int)pRenderer->Size().X(),
		(int)pRenderer->Size().Y(),
		RGB(40, 40, 40));
}

void CTableObject::Release()
{
}

void CTableObject::Update_Collider()
{
	CObject::Update_Collider();

	pCollider->Left(pCollider->Left() + rColliderOffset.left);
	pCollider->Right(pCollider->Right() + rColliderOffset.right);
	pCollider->Top(pCollider->Top() + rColliderOffset.top);
	pCollider->Bottom(pCollider->Bottom() + rColliderOffset.bottom);
}

void CTableObject::OnCollision(CObject* pObj, Vector2 vDiff)
{
	ObjectType oType = pObj->Get_ObjType();

	switch (oType)
	{
	case O_PLBULLET: case O_ENBULLET:
	if (!bInvincible)
	{
		iHitCount++;
		pCollisionBullet = pObj;
		bInvincible = true;
		dwInvicibleTime = GetTickCount();
	}
	static_cast<CBullet*>(pObj)->OnCollision_MapGround();
	if (iHitCount == 3)
	{
		eState = Broken;
	}
	break;
	case O_PLAYER :
		OnCollision_Entity(pObj, vDiff);
	}
}

void CTableObject::OnCollision_Entity(CObject* pObj, Vector2 vDiff)
{

	if ((vDiff.X() != 0.f && vDiff.Y() != 0.f) && pObj != nullptr)
	{
		// 충돌 확정
		if (bCanMove)
		{
			Table_Move(pObj, vDiff);
		}
		else
		{
			Table_Stable(pObj, vDiff);
		}

	}
}

void CTableObject::Table_Idle()
{
	if (bNoDamage)
	{
		Vector2 vDiff = Check_DetectBound();

		if (vDiff.X() != 0.f && vDiff.Y() != 0.f)
		{
			eState = TableState::Interact;

			RECT playerCollider = *pPlayer->Get_Collider()->Get_PCollider();
			RECT tableCollider = *pCollider->Get_PCollider();

			if (vDiff.X() > vDiff.Y()) // 상하
			{
				if (playerCollider.bottom <= tableCollider.top) // 플레이어가 위
				{
					interactDir = D_UP;
				}
				else
				{
					interactDir = D_DOWN;
				}
			}
			else // 좌우
			{
				if (playerCollider.right < tableCollider.left) // 플레이어가 왼쪽
				{
					interactDir = D_LEFT;
				}
				else
				{
					interactDir = D_RIGHT;
				}
			}
		}
	}
	else
	{
		iAnimRow = 2;
	}

	animKey = L"Table_IDLE";
}

void CTableObject::Table_Interact()
{
	if (!bNoDamage) return;

	// 범위 내 충돌 감지
	Vector2 vDiff = Check_DetectBound();

	if (vDiff.X() != 0.f && vDiff.Y() != 0.f)
	{
		iAnimRow = 1;
		iAnimCol = Get_TableDir();

		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E'))
		{
			eState = Flipping;
			dwFlippingTime = GetTickCount();
			iAnimRow = 1;
			iAnimCol = 0;

			switch (interactDir)
			{
			case D_UP:
				animKey = L"Table_DOWN";
				break;
			case D_DOWN:
				animKey = L"Table_UP"; 
				break;
			case D_LEFT:
				animKey = L"Table_LEFT";
				break;
			case D_RIGHT:
				animKey = L"Table_RIGHT";
				break;
			}
		}
	}
	else
	{
		eState = TableState::Idle;
		iAnimRow = 0;
		iAnimCol = 0;
	}
}

void CTableObject::Table_Flipping()
{
	if (!bNoDamage) return;

	if (dwFlippingTime + iFlippingFrameTime < GetTickCount())
	{
		iAnimCol++;
		dwFlippingTime = GetTickCount();

		if (iAnimCol >= iAnimEndCol)
		{
			eState = Flipped;
			iAnimRow = 0;
			iAnimCol = 0;
			bCanMove = true;
		}
	}
}

void CTableObject::Table_Flipped()
{
	switch (interactDir)
	{
	case D_UP:
		rColliderOffset = { 0, 0, 0, 0 };
		break;
	case D_DOWN:
		rColliderOffset = { 0, 0, 0, 0 };
		break;
	case D_LEFT:
		rColliderOffset = { 50, -10, 0, 10 };
		break;
	case D_RIGHT:
		rColliderOffset = { 0, -10, -50, 10 };
		break;
	}
	iAnimCol = 0;
	iAnimRow = 0;
}

void CTableObject::Table_Broken()
{
	iAnimRow = 2;
	switch (iHitCount)
	{
	case 0:case 1 :
		iAnimCol = 0;
		break;
	case 2: case 3 : case 4:
		iAnimCol = 1;
		break;
	case 5: case 6: case 7: 
		iAnimCol = 2;
		break;
	case 8: case 9: case 10: 
		iAnimCol = 3;
		break;
	}
}

int CTableObject::Get_TableDir()
{
	switch (interactDir)
	{
	case D_UP: 
		iAnimCol = 0; 
		break;
	case D_DOWN:
		iAnimCol = 1; 
		break;
	case D_LEFT:
		iAnimCol = 2; 
		break;
	case D_RIGHT:
		iAnimCol = 3; 
		break;
	}
	return iAnimCol;
}

void CTableObject::Table_Move(CObject* pObj, Vector2 vDiff)
{
	RECT playerCollider = *pObj->Get_Collider()->Get_PCollider();
	RECT tableCollider = *pCollider->Get_PCollider();

	if (playerCollider.bottom - 10 <= tableCollider.top) // 플레이어가 테이블 위
	{
		pTransform->Position({ pTransform->Position().X(), pTransform->Position().Y() + (/*tableCollider.top - playerCollider.bottom + */6) });
	}
	else if (playerCollider.top + 10 >= tableCollider.bottom) // 플레이어가 테이블 아래 
	{
		pTransform->Position({ pTransform->Position().X(), pTransform->Position().Y() - (/*tableCollider.top - playerCollider.bottom + */6) });
	}
	else if (animKey == L"Table_RIGHT")
	{
		if (playerCollider.right - 10 <= tableCollider.left) // player is left side
		{
			pTransform->Position({ pTransform->Position().X() + 6, pTransform->Position().Y() });
		}
		else if (playerCollider.left + 30 <= tableCollider.right)
		{
			pTransform->Position({ pTransform->Position().X() - 6, pTransform->Position().Y() });
		}
	}
	else 	if (animKey == L"Table_LEFT")
	{
		if (playerCollider.right - 10 >= tableCollider.left) // player is left side
		{
			pTransform->Position({ pTransform->Position().X() + 6, pTransform->Position().Y() });
		}
		else if (playerCollider.left + 5 >= tableCollider.right)
		{
			pTransform->Position({ pTransform->Position().X() - 6, pTransform->Position().Y() });
		}
	}
	else
	{
		if (playerCollider.right - 10 <= tableCollider.left) // player is left side
		{
			pTransform->Position({ pTransform->Position().X() + 6, pTransform->Position().Y() });
		}
		else if (playerCollider.left + 10 >= tableCollider.right)
		{
			pTransform->Position({ pTransform->Position().X() - 6, pTransform->Position().Y() });
		}
	}
}

void CTableObject::Table_Stable(CObject* pObj, Vector2 vDiff)
{
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

Vector2 CTableObject::Check_DetectBound()
{
	RECT playerCollider = *pPlayer->Get_Collider()->Get_PCollider();
	RECT tableCollider = *pCollider->Get_PCollider();

	Vector2 playerPos = pPlayer->Get_Transform()->Position();
	Vector2 playerColSize = pPlayer->Get_Collider()->Size();

	float fDistX = abs(playerPos.X() - pTransform->Position().X());
	float fDistY = abs(playerPos.Y() - pTransform->Position().Y());

	float fRadX = playerColSize.X() * 0.5f + (pTransform->Position().X() - rDetectBound.left);
	float fRadY = playerColSize.Y() * 0.5f + (pTransform->Position().Y() - rDetectBound.top);

	Vector2 vDiff{};
	if ((fRadX >= fDistX) && (fRadY >= fDistY))
	{
		vDiff = { fRadX - fDistX, fRadY - fDistY };
	}
	return vDiff;
}
