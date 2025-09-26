#include "pch.h"
#include "CMob.h"
#include "CState.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CMobStateMachine.h"
#include "CRelease.h"

CMob::CMob()
{
	pPlayer = nullptr;
	vPlayerPos = {};
	iMaxHP = -1.f;
	iHP = iMaxHP;
	pStateMachine = nullptr;
	pCurrentState = nullptr;
}

CMob::~CMob()
{
	Release();
}

void CMob::Initialize()
{
	CObject::Initialize();
	pRenderer->rType = RND__GAMEBOJECT;
	eType = O_ENEMY;

	pStateMachine = new CMobStateMachine(this);
	pStateMachine->Initialize();
}

int CMob::Update()
{
    return 0;
}

void CMob::Late_Update()
{
}

void CMob::Render(HDC _hDC)
{
}

void CMob::Release()
{
	CRelease<CMobStateMachine*>::Release(pStateMachine);
}

void CMob::Update_Transform()
{
	Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
							 pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
	pTransform->Position(move(vNew));
}

void CMob::Update_Renderer()
{
	CObject::Update_Renderer();
}

void CMob::OnCollision(CObject* pObj, Vector2 vDiff)
{
	ObjectType type = pObj->Get_ObjType();
	if (type == O_PLBULLET)
	{
		this;
		iHP = (iHP - 1 <= 0 ? 0 : iHP - 1);
	}
}