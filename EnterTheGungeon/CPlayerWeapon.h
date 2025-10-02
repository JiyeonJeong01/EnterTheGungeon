#pragma once
#include "CWeapon.h"

class CPlayerWeapon :  public CWeapon
{
public :
	enum GunType { };
public :
	CPlayerWeapon(CObject* pObj = nullptr) : CWeapon(pObj) {}
	~CPlayerWeapon() override;
public :
	void Initialize() override;
	void Attack() override;
	void Change_Weapon(WeaponType wType);
	void Swap_Weapon();
	void Pistol_Attack();
	void Shotgun_Attack();
	void Reload();

public :
	int Get_CurPistolBullet() { return iPistolCurBullet; }
	int Get_CurShotgunBullet() { return iShotgunCurBullet; }

private :
	int iPistolMaxBullet = 10;
	int iPistolCurBullet;
	float fPistolLimitFireTime;

	int iShotgunMaxBullet = 5;
	int iShotgunCurBullet;
	float fShotgunLimitFireTime;

	bool bCanShotGun;
};

