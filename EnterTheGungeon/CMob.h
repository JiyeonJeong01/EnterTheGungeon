#pragma once
#include "CObject.h"

class CPlayer;
class CMobStateMachine;
class CState;

class CMob :  public CObject
{
public :
	enum MobType { Mob01, Mob02, Mob03, Mob04 };
	enum MobState { Idle, Walk, Attack, Damaged, Dead, MobEnd};
public :
	CMob();
	~CMob() override;

public: // Core
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public: // Components
	void Update_Transform() override;
	void Update_Renderer() override;

public:
	void OnCollision(CObject* pObj, Vector2 vDiff) override;
	
public :
	void Check_ShouldAtack();
	void Check_Invincible();

	void Change_KnockbackState();

public :
	float Get_Speed() { return fSpeed;  }
	void Set_Speed(float fSpeed) { this->fSpeed = fSpeed; }

	int Get_HP() { return iHP; }
	void Modify_HP(int _iHP) {
		this->iHP = (iHP + _iHP >= 0) ? iHP + _iHP : 0;
	}
	
	void Set_Dead() { bAlive = false; }

protected :
	// Player
	CPlayer* pPlayer;
	Vector2 vPlayerPos;

	// Stats
	int iMaxHP;
	int iHP;

public:
	float fAttackTimeRange;
	float fSpeed;

	float fInvincibleTime;
	DWORD dwInvincibleTime;
	bool bInvincible;
	bool bKnockback;

protected:
	CMobStateMachine* pStateMachine;
	CState* pCurrentState;
	MobState eCurrentState;


};

