#pragma region INCLUDE
#include "pch.h"
#include "CMob03WalkState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#pragma endregion


CMob03WalkState::CMob03WalkState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob03WalkState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 2.0f;
	fCurStateMaxTime = 5.f;
	dwLastAttackTime = GetTickCount();
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_ATTACK.bmp", L"Monster03_ATTACK");

	animation.Initialize(0, 7, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);

	Pre_Calculate();
}

void CMob03WalkState::Update()
{
	CMobState::Update();
	Update_AnimFrame();
	
	if (bAttackTimer)
	{
		Do_Attack();
	}

}

void CMob03WalkState::Late_Update()
{
	if (dwLastAttackTime + fAttackTimeRange * 1000 < GetTickCount())
	{
		bAttackTimer = true;
	}
}

void CMob03WalkState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster03_ATTACK");

	CRenderer renderer = *(pObj->Get_Renderer());
	int realSize = 230;
	int iRenderSize = 230;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		iRenderSize, iRenderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		0,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob03WalkState::Release()
{
}

void CMob03WalkState::Exit()
{

}

void CMob03WalkState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
	bAttackTimer = true;
	dwLastAttackTime = GetTickCount();
	iCurrentAttackCount = 0;

}

void CMob03WalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob03WalkState::Stop_Animation()
{
}

void CMob03WalkState::On_End_Animation()
{
}

int CMob03WalkState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}

void CMob03WalkState::Do_Attack()
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
	iCurrentAttackCount++;
	bAttackTimer = false;
	dwLastAttackTime = GetTickCount();
}

void CMob03WalkState::Do_KnockBack()
{
}

void CMob03WalkState::Attack01()
{
	int iDist = 30;

	for (auto vDir : vNormalizedDir01)
	{
		float fX = vDir.X() * iDist;
		float fY = vDir.Y() * iDist;

		CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
			O_ENBULLET, pObj->Get_Transform()->Position().X() + fX, pObj->Get_Transform()->Position().Y() + fY));

		pBullet->Set_BulletType(CBullet::B02);
		pBullet->Set_EffectType(CBullet::E04);
		pBullet->Apply_BulletSprite();
		pBullet->Apply_EffectAnim();
		pBullet->Set_Direction(vDir);
		pBullet->Set_Speed(6.f);
	}
}

void CMob03WalkState::Attack02()
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

	Vector2 crossDirs[4] = {	{ 1,  0}, {-1,  0}, { 0,  1}, { 0, -1}};

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

void CMob03WalkState::Attack03()
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

void CMob03WalkState::Pre_Calculate()
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
