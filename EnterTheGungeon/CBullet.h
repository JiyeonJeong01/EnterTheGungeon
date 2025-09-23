#pragma once
#include "CObject.h"
#include "CAnimation.h"

class CBullet :   public CObject
{
public :
	enum BulletType { B01, B02, B03, B04, BossBullet, Boss_Cheese, Boss_Summon, Boss_Kunai, B_END};
	enum EffectType { E01, E02, E03, E04, E05, E_END};
public:
	CBullet();
	~CBullet() override;

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
	void Apply_BulletSprite();
	void Apply_EffectAnim();

public :
	void Set_Speed(float fSpeed) { this->fSpeed = fSpeed; }
	void Set_Direction(Vector2 vDir);
	void Set_BulletType(BulletType type) { eBulletType = type; }
	void Set_EffectType(EffectType type) { eEffectType = type; }

public :
	bool Check_InBound();
	void OnCollision(CObject* pObj);
	void OnCollision_MapGround();

protected :
	float fSpeed;

	BulletType eBulletType;
	EffectType eEffectType;

	const TCHAR* spriteKey;
	const TCHAR* bulletSpriteKey;
	const TCHAR* effetAnimKey;
	CAnimation effectAnim;
	
public :
	bool bCanRend;

};

