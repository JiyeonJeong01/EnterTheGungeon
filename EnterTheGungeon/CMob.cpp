#include "pch.h"
#include "CMob.h"
#include "CState.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CMobStateMachine.h"
#include "CMob01StateMachine.h"
#include "CStageManager.h"
#include "CRelease.h"

CMob::CMob()
{
	pPlayer = nullptr;
	vPlayerPos = {};
	iMaxHP = -1.f;
	iHP = iMaxHP;
	pStateMachine = nullptr;
	pCurrentState = nullptr;
	fInvincibleTime = 0.3f;
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

	bInvincible = false;
	bKnockback = false;

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
		if (!bInvincible)
		{
			this;
			bInvincible = true;
			bKnockback = true;
			dwInvincibleTime = GetTickCount();
			iHP--;
			if (iHP <= 0)
			{
				iHP = 0;
			}
			// TODO  !!!!!!!!!!!!!!!!!!!!!!!! 수정할 거 이ㅏㄹ멍너ㅏㅣㅇㄴ러ㅏㅣㄴㄹ아ㅣㅓㅏㅓㅣㅇㄴㄹ
		}
	}
}

void CMob::Check_Invincible()
{
	if (bInvincible && dwInvincibleTime + fInvincibleTime * 1000 < GetTickCount())
	{
		dwInvincibleTime = GetTickCount();
		bInvincible = false;
	}
}

void CMob::Change_KnockbackState()
{
	bKnockback = true;
	iHP--;
	if (iHP <= 0)
	{
		iHP = 0;
	}
	printf("몬스터 현재 체력 : %d\n", iHP);
	this;
	pStateMachine;
	pStateMachine->Change_State(MobState::Damaged);
}
