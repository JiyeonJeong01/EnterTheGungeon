#include "pch.h"
#include "CPlayerWeapon.h"
#include "CPlayerBullet.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CInputManager.h"
#include "CPlayerState.h"
#include "CReloadBar.h"
#include "CPlayer.h"
#include "CSoundManager.h"

CPlayerWeapon::~CPlayerWeapon()
{
	Release();
}

void CPlayerWeapon::Initialize()
{
	wType = PG01;
	iPistolCurBullet = iPistolMaxBullet;
	iShotgunCurBullet = iShotgunMaxBullet;
}

void CPlayerWeapon::Attack()
{
	if (wType == WeaponType::PG01)
	{
		Pistol_Attack();
	}
	else if (wType == WeaponType::PG02)
	{
		Shotgun_Attack();
	}
}

void CPlayerWeapon::Change_Weapon(WeaponType wType)
{
	this->wType = wType;

	if (wType == WeaponType::PG01)
	{
		iPistolCurBullet = iPistolMaxBullet;
	}
	if (wType == WeaponType::PG02)
	{
		iShotgunCurBullet = iShotgunMaxBullet;
	}
}

void CPlayerWeapon::Swap_Weapon()
{
	if (wType == WeaponType::PG01)
	{
		Change_Weapon(WeaponType::PG02);
	}
	else if (wType == WeaponType::PG02)
	{
		Change_Weapon(WeaponType::PG01);
	}
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"Player_WeaponSwap.wav", 1.f);

}

void CPlayerWeapon::Pistol_Attack()
{
	if (--iPistolCurBullet <= 0)
	{
		static_cast<CPlayer*>(pOwner)->bReloading = true;
		static_cast<CPlayer*>(pOwner)->pReloadBar->Start_Reload();
	}
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"Player_Shot01.wav", 1.f);

	CPlayerBullet* pBullet = dynamic_cast<CPlayerBullet*>(CObjectFactory<CPlayerBullet>::Create(
		O_PLBULLET, pOwner->Get_Transform()->Position().X(), pOwner->Get_Transform()->Position().Y()));

	pBullet->Set_BulletType(CBullet::B03);
	pBullet->Set_EffectType(CBullet::E01);
	pBullet->Apply_BulletSprite();
	pBullet->Apply_EffectAnim();

	MANAGER(CCameraManager*, M_CAMERA)->Set_CamerMode(CCameraManager::Shake);

	POINT curPos = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 curRealPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)curPos.x, (float)curPos.y });
	Vector2 dir = { pOwner->Get_Transform()->Position().X() - (float)curRealPos.X(),  pOwner->Get_Transform()->Position().Y() - (float)curRealPos.Y() };
	dir.Normalize();
	pBullet->Set_Direction(dir * -1.f);
	pBullet->Set_Speed(17.f);
	pBullet->Set_ObjType(O_PLBULLET);
}

void CPlayerWeapon::Shotgun_Attack()
{
	if (--iShotgunCurBullet <= 0)
	{
		static_cast<CPlayer*>(pOwner)->bReloading = true;
		static_cast<CPlayer*>(pOwner)->pReloadBar->Start_Reload();
	}
	MANAGER(CCameraManager*, M_CAMERA)->Set_CamerMode(CCameraManager::Shake);

	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"Player_Shot02.wav", 1.f);


	POINT curPos = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 curRealPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)curPos.x, (float)curPos.y });

	Vector2 originDir = { pOwner->Get_Transform()->Position().X() - (float)curRealPos.X(),  pOwner->Get_Transform()->Position().Y() - (float)curRealPos.Y() };
	originDir.Normalize();

	Vector2 vOffset = { originDir.X() * 20.f,  originDir.Y() };

	CPlayerBullet* pBullet1 = dynamic_cast<CPlayerBullet*>(CObjectFactory<CPlayerBullet>::Create(
		O_PLBULLET, pOwner->Get_Transform()->Position().X() - originDir.X() * 50.f, pOwner->Get_Transform()->Position().Y()));
	CPlayerBullet* pBullet2 = dynamic_cast<CPlayerBullet*>(CObjectFactory<CPlayerBullet>::Create(
		O_PLBULLET, pOwner->Get_Transform()->Position().X() - originDir.X() * 50.f, pOwner->Get_Transform()->Position().Y()));
	CPlayerBullet* pBullet3 = dynamic_cast<CPlayerBullet*>(CObjectFactory<CPlayerBullet>::Create(
		O_PLBULLET, pOwner->Get_Transform()->Position().X() - originDir.X() * 50.f, pOwner->Get_Transform()->Position().Y()));

	pBullet1->Set_BulletType(CBullet::B01);
	pBullet1->Set_EffectType(CBullet::E05);
	pBullet1->Apply_BulletSprite();
	pBullet1->Apply_EffectAnim();

	pBullet2->Set_BulletType(CBullet::B01);
	pBullet2->Set_EffectType(CBullet::E05);
	pBullet2->Apply_BulletSprite();
	pBullet2->Apply_EffectAnim();

	pBullet3->Set_BulletType(CBullet::B01);
	pBullet3->Set_EffectType(CBullet::E05);
	pBullet3->Apply_BulletSprite();
	pBullet3->Apply_EffectAnim();

	pBullet1->Set_Direction(originDir * -1.f);
	pBullet1->Set_Speed(17.f);
	pBullet1->Set_ObjType(O_PLBULLET);

	float degree = 20.0f;
	float radian = degree * (3.141592f / 180.0f);

	float cosTheta = cosf(radian);
	float sinTheta = sinf(radian);

	Vector2 leftDir = { originDir.X() * cosTheta - originDir.Y() * sinTheta, originDir.X() * sinTheta + originDir.Y() * cosTheta };
	pBullet2->Set_Direction(leftDir * -1.f);
	pBullet2->Set_Speed(17.f);
	pBullet2->Set_ObjType(O_PLBULLET);

	Vector2 rightDir = { originDir.X() * cosTheta + originDir.Y() * sinTheta, -originDir.X() * sinTheta + originDir.Y() * cosTheta };
	pBullet3->Set_Direction(rightDir * -1.f);
	pBullet3->Set_Speed(17.f);
	pBullet3->Set_ObjType(O_PLBULLET);

}

void CPlayerWeapon::Reload()
{
	if (wType == WeaponType::PG01)
	{
		iPistolCurBullet = iPistolMaxBullet;
	}
	if (wType == WeaponType::PG02)
	{
		iShotgunCurBullet = iShotgunMaxBullet;
	}
}