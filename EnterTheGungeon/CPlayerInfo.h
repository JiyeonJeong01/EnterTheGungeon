#pragma once
#include "CObject.h"
class CPlayer;
class CPlayerInfo : public CObject
{
public:
	CPlayerInfo();
	~CPlayerInfo() override;

public:
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
	void Set_Player(CPlayer* pPlayer) { this->pPlayer = pPlayer; }

	int Get_PlayerHP() const { return iPlayerHP; }
	void Set_PlayerHP(int hp) { iPlayerHP = hp; }

	int Get_PlayerMaxHP() const { return iPlayerMaxHP; }
	void Set_PlayerMaxHP(int maxHp) { iPlayerMaxHP = maxHp; }

	int Get_Cartridge() const { return iCartridge; }
	void Set_Cartridge(int count) { iCartridge = count; }

	int Get_Coin() const { return iCoin; }
	void Set_Coin(int count) { iCoin = count; }

	int Get_Key() const { return iKey; }
	void Set_Key(int count) { iKey = count; }

private:
	CPlayer* pPlayer;
	RECT rBarBound;

public :
	int iPlayerHP;
	int iPlayerMaxHP;
	int iCartridge;
	int iCoin;
	int iKey;
	int iBulletCount;
};

