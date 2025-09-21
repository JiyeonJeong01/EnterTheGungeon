#pragma once
#include "CManager.h"

class CPlayer;
class CInventory :  public CManager
{
public :
	CInventory(CPlayer* pPlayer = nullptr) : pPlayer(pPlayer) {}
	~CInventory() override;

public :
	void Initialize() override;
	void Update() override;
	void Release() override;

private :
	CPlayer* pPlayer;

	int iHP;
	int iCartridge;



};

