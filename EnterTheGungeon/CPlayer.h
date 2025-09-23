#pragma once
#include "CObject.h"

class CPlayerStateMachine;
class CState;
class CStats;
class CPlayerWeapon;
class CInventory;
class CReloadBar;
class CPlayerInfo;

class CPlayer : public CObject
{
public :
	enum PlayerState { PS_IDLE, PS_WALK, PS_ATTACK, PS_HIT, PS_DODGE, PS_FALL, PS_DEAD, PS_END};
public:
	CPlayer();
	virtual ~CPlayer();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public: // Components
	void Update_Transform() override;
	void Update_Renderer() override;

public :
	void Initialize_PlayerComponents();
	void Handle_Input();
	void Finist_Reloading();

public :
	void OnCollision(CObject* pObj, Vector2 vDiff) override;
	void OnCollision_EnBullet(CObject* pObj, Vector2 vDiff);

public :
	void Set_CurrentState(PlayerState eState, CState* pState) { eCurrentState = eState; pCurrentState = pState; }

public:
	Vector2 vInputDir;
	POINT pInputCursor;
	DWORD dwLastFireTime;
	float fLimitFireTime;
	float fSpeed;
	bool bDodgePlaying;
	Direction eDir;
	bool bReloading;

public :
	int iHP;
	int iMaxHP;

private :
	CPlayerStateMachine* pStateMachine;
	CState* pCurrentState;
	PlayerState eCurrentState;

public :
	CPlayerWeapon* pWeapon;
	CInventory* pInventory;
	CReloadBar* pReloadBar;
	CPlayerInfo* pPlayerInfo;

};

