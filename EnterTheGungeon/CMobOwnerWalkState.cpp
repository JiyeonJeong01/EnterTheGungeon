#pragma region INCLUDE
#include "pch.h"
#include "CMobOwnerWalkState.h"
#include "CBmpManager.h"
#include "CObjectFactory.h"

#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CSoundManager.h"

#include "CStateMachine.h"
#include "CMobBullet.h"
#pragma endregion


CMobOwnerWalkState::CMobOwnerWalkState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMobOwnerWalkState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 2.0f;
	fCurStateMaxTime = 5.f;
	dwLastAttackTime = GetTickCount();
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;

	animation.Initialize(0, 4, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 120);
	iAnimRow = 3;
	Pre_Calculate();
}

void CMobOwnerWalkState::Update()
{
	CMobState::Update();
	Update_AnimFrame();

	if (bAttackTimer)
	{
		Do_Attack();
	}

}

void CMobOwnerWalkState::Late_Update()
{
	if (dwLastAttackTime + fAttackTimeRange * 1000 < GetTickCount())
	{
		bAttackTimer = true;
	}
}

void CMobOwnerWalkState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Owner");

	CRenderer renderer = *(pObj->Get_Renderer());
	int realSize = 200;
	int iRenderSize = 200;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		iRenderSize, iRenderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		iAnimRow * realSize,
		realSize, realSize,
		RGB(53, 53, 53));
}

void CMobOwnerWalkState::Release()
{
}

void CMobOwnerWalkState::Exit()
{

}

void CMobOwnerWalkState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	fAttackTimeRange = 2.f;
	bAttackTimer = false;
	dwLastAttackTime = GetTickCount();
	iCurrentAttackCount = 0;

}

void CMobOwnerWalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMobOwnerWalkState::Stop_Animation()
{
}

void CMobOwnerWalkState::On_End_Animation()
{
	if (iAnimRow == 3)
	{
		iAnimRow = 4;
		fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 100);
		animation.Initialize(0, 5, (int)D_DOWN);
		bAttackTimer = true;
	}
}

int CMobOwnerWalkState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}

void CMobOwnerWalkState::Do_Attack()
{
	if (!bCanAttack) return;

	if (iCurrentAttackCount % 3 == 0)
	{
		Attack01();
	}
	else if (iCurrentAttackCount % 3 == 1)
	{
		Attack02();
	}
	else
	{
		Attack03();
	}
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"CombineBomb_Shot_01.wav", 1.f);

	iCurrentAttackCount++;
	bAttackTimer = false;
	dwLastAttackTime = GetTickCount();
}

void CMobOwnerWalkState::Do_KnockBack()
{
}

void CMobOwnerWalkState::Attack01()
{
	int bulletCount = 40;
	float angleRange = 90.f;
	float angleStep = angleRange / (bulletCount - 1);
	vDirToPlayer.Normalize();
	Vector2 vBaseDir = vDirToPlayer;

	float startAngle = -angleRange / 2.f;
	for (int i = 0; i < bulletCount; i++)
	{
		float angleDeg = startAngle + i * angleStep;
		float angleRad = angleDeg * (PI / 180.f);
		float cosA = cosf(angleRad);
		float sinA = sinf(angleRad);

		Vector2 vRotatedDir(vBaseDir.X() * cosA - vBaseDir.Y() * sinA, vBaseDir.X() * sinA + vBaseDir.Y() * cosA);

		CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
			O_ENBULLET, pObj->Get_Transform()->Position().X() + vRotatedDir.X() * 5.f, pObj->Get_Transform()->Position().Y() + vRotatedDir.Y() * 5.f));
		
		pBullet->Set_BulletType(CBullet::B02);
		pBullet->Set_EffectType(CBullet::E04);
		pBullet->Apply_BulletSprite();
		pBullet->Apply_EffectAnim();
		pBullet->Set_Direction(vRotatedDir);
		pBullet->Set_Speed(5.f);
	}
}

void CMobOwnerWalkState::Attack02()
{
	int iDist = 25;

	for (auto vDir : vNormalizedDir01)
	{
		float fX = vDir.X() * iDist;
		float fY = vDir.Y() * iDist;

		CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
			O_ENBULLET,
			pObj->Get_Transform()->Position().X() + fX,
			pObj->Get_Transform()->Position().Y() + fY));

		pBullet->Set_BulletType(CBullet::B02);
		pBullet->Set_EffectType(CBullet::E04);
		pBullet->Apply_BulletSprite();
		pBullet->Apply_EffectAnim();
		pBullet->Set_Direction(vDir);
		pBullet->Set_Speed(6.f);
	}

	Vector2 crossDirs[4] = { { 1,  0}, {-1,  0}, { 0,  1}, { 0, -1} };

	for (auto vDir : crossDirs)
	{
		for (int i = 0; i < 4; i++)
		{
			float fX = vDir.X() * (iDist + i * 25);
			float fY = vDir.Y() * (iDist + i * 25);

			CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
				O_ENBULLET,
				pObj->Get_Transform()->Position().X() + fX,
				pObj->Get_Transform()->Position().Y() + fY));

			pBullet->Set_BulletType(CBullet::B02);
			pBullet->Set_EffectType(CBullet::E04);
			pBullet->Apply_BulletSprite();
			pBullet->Apply_EffectAnim();
			pBullet->Set_Direction(vDir);
			pBullet->Set_Speed(6.f);
		}
	}
}

void CMobOwnerWalkState::Attack03()
{
	int iDist = 25;
	Vector2 crossDirs[4] = { {1,1}, {-1,1}, {1,-1}, {-1,-1} };

	for (auto vDir : crossDirs)
	{
		vDir.Normalize();


		for (int i = 0; i < 20; i++)
		{
			float fX = vDir.X() * (iDist + i * 35);
			float fY = vDir.Y() * (iDist + i * 35);

			CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
				O_ENBULLET,
				pObj->Get_Transform()->Position().X() + fX,
				pObj->Get_Transform()->Position().Y() + fY));

			pBullet->Set_BulletType(CBullet::B02);
			pBullet->Set_EffectType(CBullet::E04);
			pBullet->Apply_BulletSprite();
			pBullet->Apply_EffectAnim();
			pBullet->Set_Direction(vDir);
			pBullet->Set_Speed(6.f);
		}
	}
}

void CMobOwnerWalkState::Pre_Calculate()
{
	int iBulletCount = 50;
	int iDegree = 360 / iBulletCount;
	int iCurDegree = 0;
	int iDist = 30;

	for (int i = 0; i < iBulletCount; ++i)
	{
		float fRad = iCurDegree * PI / 180.f;
		float fX = cosf(fRad) * iDist;
		float fY = sinf(fRad) * iDist;

		Vector2 vDir = { fX, fY };
		vDir.Normalize();
		iCurDegree += iDegree;

		vNormalizedDir01.push_back(vDir);
	}
}
