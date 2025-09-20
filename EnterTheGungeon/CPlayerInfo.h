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

public:
	void Set_Player(CPlayer* pPlayer) { this->pPlayer = pPlayer; }

private:
	CPlayer* pPlayer;
	RECT rBarBound;

public :
	int iPlayerHP;
	int iCartridge;

};

