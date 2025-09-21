#pragma once

#include "CObject.h"

class CBullet;

class CWeapon : public CObject
{
public:
	enum WeaponType { PG01, PG02, EG01, EG02, EG03 };
	enum EffectType { E01, E02, E03, E04, E05, E_END };
public:
	CWeapon(CObject* pObj = nullptr);
	~CWeapon() override;

public:
	static void Load_Resource();
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public: // Components
	void Update_Transform() override;
	void Update_AnimFrame();

public :
	virtual void Attack();

public:
	void Apply_WeaponSprite();
	void Apply_EffectAnim();

public:
	void Set_WeaponType(WeaponType type) { wType = type; }
	void Set_EffectType(EffectType type) { eType = type; }

	WeaponType Get_WeaponType() { return wType;  }

protected:
	CObject* pOwner;

protected:
	WeaponType wType;
	EffectType eType;
};

