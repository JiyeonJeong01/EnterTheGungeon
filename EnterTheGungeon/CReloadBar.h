#pragma once
#include "CObject.h"
class CPlayer;
class CReloadBar : public CObject
{
public:
	CReloadBar();
	~CReloadBar() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void Update_Transform() override {};
	void Update_Renderer() override;
	
public :
	void Start_Reload();

public : 
	void Set_Player(CPlayer* pPlayer) { this->pPlayer = pPlayer; }

public:
	bool bActive;

private :
	CPlayer* pPlayer;
	Vector2 pPlayerScreenPos;
	RECT rBarBound;
	POINT pScrollPos;

	float fReloadTime;
	DWORD dwTime;
	int iCurMoveDist;
	int iMoveDistPerFrame;

};