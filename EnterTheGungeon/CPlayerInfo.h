#pragma once
#include "CObject.h"
class CPlayer;
class CPlayerInfo : public CObject
{
public:
	CPlayerInfo();
	~CPlayerInfo() override;

public:
	static void Load_Resource();
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void Update_Transform() override {};
	void Update_Renderer() override;

public :
	void Draw_HP(HDC hDC);
	void Draw_WeaponType(HDC hDC);
	void Draw_BulletCount(HDC hDC);

public:
	void Shake();

public:
	void Set_Player(CPlayer* pPlayer) { this->pPlayer = pPlayer; }

	int Get_PlayerHP() const { return iPlayerHP; }
	void Set_PlayerHP(int hp) { iPlayerHP = hp; }

	int Get_PlayerMaxHP() const { return iPlayerMaxHP; }
	void Set_PlayerMaxHP(int maxHp) { iPlayerMaxHP = maxHp; }

private:
	CPlayer* pPlayer;
	RECT rBarBound;

private :
	int iPlayerHP;
	int iPlayerMaxHP;
	int iBulletCount;
};

