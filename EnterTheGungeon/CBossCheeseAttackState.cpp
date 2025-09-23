#include "pch.h"
#include "CBossCheeseAttackState.h"
#include "CBmpManager.h"
#include "CSoundManager.h"
#include "CObjectFactory.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CBossStateMachine.h"
#include "CBossBullet.h"

void CBossCheeseAttackState::Initialize()
{
	CBossState::Initialize();

	eState = CBoss::BS_CHEESEATTACK;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_CheeseAttack.bmp", L"Boss_CheeseAttack");
	animation.Initialize(0, 23, (int)D_DOWN);

	fCurAttackDegree = 90.f;
	iPrevFrame = -1;
	fCurAttackDegree = 90.f;

	dwLastFireTime = GetTickCount();
}

void CBossCheeseAttackState::Update()
{
	CBossState::Update();
	Update_AnimFrame();
}

void CBossCheeseAttackState::Late_Update()
{
}

void CBossCheeseAttackState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_CheeseAttack");

	CRenderer renderer = *(pObj->Get_Renderer());

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)renderer.Size().X(),
		0,
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		RGB(255, 0, 255));
}

void CBossCheeseAttackState::Release()
{
}

void CBossCheeseAttackState::Exit()
{
	static_cast<CBoss*>(pObj)->Set_LastAttackTime();
}

void CBossCheeseAttackState::Enter()
{
	fCurAttackDegree = 90.f;
	iPrevFrame = -1;
	dwLastFireTime = 0;

	iRandAttack = rand() % 2;

	if (iRandAttack)
	{
		fFireDegreeRange = 12.f;
		iAnimLoopCount = 0;
		int iFrameRange = 70;
		fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), iFrameRange);
		fFireTimeRange = (iFrameRange * 11) / (360.f / fFireDegreeRange);
	 bAnimLoop = true;
	}
	else
	{
		fFireDegreeRange = 60.f;

		int iFrameRange = 80;
		fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), iFrameRange);
		fFireTimeRange = (iFrameRange * 10) / (360.f / fFireDegreeRange);
	 bAnimLoop = false;
	}
}

void CBossCheeseAttackState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
	if (iPrevFrame != animation.iCurrIndex && animation.iCurrIndex >= 10 && animation.iCurrIndex <= 20)
	{
		if (iRandAttack)
		{
			Attack_CheeseBullet();
		}
		else
		{
			Attack_CircleBullet();
		}
		iPrevFrame = animation.iCurrIndex;
		dwLastFireTime = GetTickCount();
	}
	if (animation.iCurrIndex == 19 && bAnimLoop && iAnimLoopCount < 3)
	{
		animation.iCurrIndex = 10;
		iAnimLoopCount++;
		if (iAnimLoopCount >= 3)
		{
			bAnimLoop = false;
		}
	}
}

void CBossCheeseAttackState::Stop_Animation()
{
}

void CBossCheeseAttackState::On_End_Animation()
{
	pStateMachine->Change_State(CBoss::BS_IDLE);
}

int CBossCheeseAttackState::Dir_AnimRow(Direction eDir)
{
    return 0;
}

void CBossCheeseAttackState::Attack_CheeseBullet()
{
	float fRad = PI / 180.f * fCurAttackDegree;

	float dx = cosf(fRad);
	float dy = sinf(fRad);
	Vector2 vDir = { dx, dy };
	vDir.Normalize();

	CBossBullet* pBullet = static_cast<CBossBullet*>(CObjectFactory<CBossBullet>::Create(
		O_ENBULLET, pObj->Get_Transform()->Position().X() + vDir.X(), pObj->Get_Transform()->Position().Y() + vDir.Y()));
	pBullet->Set_BulletType(CBullet::Boss_Cheese);
	pBullet->Set_EffectType(CBullet::E04);
	pBullet->Apply_BulletSprite();
	pBullet->Apply_EffectAnim();

	pBullet->Set_Direction(vDir);
	pBullet->Set_Speed(23.f);
	fCurAttackDegree += fFireDegreeRange;
}

void CBossCheeseAttackState::Attack_CircleBullet()
{
	float fRad = PI / 180.f * fCurAttackDegree;
	float fDistToPlayer = 200.f;
	float dx = cosf(fRad) * fDistToPlayer;
	float dy = sinf(fRad) * fDistToPlayer;
	Vector2 vDir1 = { dx, dy };
	// 생성할 원점 
	Vector2 vOrigin = { pObj->Get_Transform()->Position().X() + vDir1.X(), pObj->Get_Transform()->Position().Y() + vDir1.Y()};


	int iBulletCount = 12;
	int iDistToOrigin = 100;
	for (int i = 0; i < iBulletCount; i++)
	{
		float fRad = PI / 180.f * (i * 30);
		float dx = cosf(fRad) * iDistToOrigin;
		float dy = sinf(fRad) * iDistToOrigin;
		Vector2 vDir = { dx, dy };
		CBossBullet* pBullet = static_cast<CBossBullet*>(CObjectFactory<CBossBullet>::Create(
			O_ENBULLET, vOrigin.X() + vDir.X(), vOrigin.Y() + vDir.Y()));
		pBullet->Set_BulletType(CBullet::BossBullet);
		pBullet->Set_EffectType(CBullet::E04);
		pBullet->Apply_BulletSprite();
		pBullet->Apply_EffectAnim();

		pBullet->Set_Direction(vDir1.Get_Normalized());
		pBullet->Set_Speed(18.f);
	}

	fCurAttackDegree = (float)(int(fCurAttackDegree + fFireDegreeRange) % 360);
}
