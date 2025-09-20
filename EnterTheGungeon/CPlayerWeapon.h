#pragma once
#include "CWeapon.h"

class CPlayerWeapon :  public CWeapon
{
public :
	CPlayerWeapon(CObject* pObj = nullptr) : CWeapon(pObj) {}
	~CPlayerWeapon() override;
public :
	void Initialize() override;
	void Attack() override;
	void Change_Weapon(WeaponType wType);
	void Pistol_Attack();
	void Shotgun_Attack();
	void Reload();

private :
	int iPistolMaxBullet = 7;
	int iPistolCurBullet;
	float fPistolLimitFireTime;

	int iShotgunMaxBullet = 5;
	int iShotgunCurBullet;
	float fShotgunLimitFireTime;

	bool bCanShot;


};

