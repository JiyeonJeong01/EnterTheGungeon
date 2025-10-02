#pragma once
#include "CState.h"
#include "CPlayer.h"

class CObject;
class CStateMachine;

class CPlayerState : public CState
{
public:
	CPlayerState(CObject* pObj, CStateMachine* pStateMachine)
		: CState(pObj, pStateMachine) {
	}
	~CPlayerState() override {};

public :
	void Update() override;

public:
	CPlayer::PlayerState Get_State() { return eState; }
	void Render_Player(HDC hDC);
	void Dir_ByCursor();
	virtual int Dir_AnimRow(Direction eDir) { return 0;  };
	void Shot_Bullet();

public :
	void Get_WeaponDir();

public :
	void Attack_Bomb();

protected:
	CPlayer::PlayerState eState;

public :
	Vector2 vInputDir;
	Direction eDir;
	POINT pInputCursor;
	DWORD dwLastFireTime;
	float fLimitFireTime;

	float fSpeed;
	bool bDodgePlaying;
	bool bCanShotGun;
	bool bCanShotBomb;
	bool bReloading = false;

	int iWeaponColIndex;
	int iWeaponRowIndex;

	bool bShouldBombEffect;
	bool bEndBombEffect;
	int iAnimColIndex, iAnimRowIndex;
	DWORD dwBombAnimElapsedTime;
	POINT pCursor;
	int iBombEffectSize = 300;


	bool bShouldShotEffect; 
	bool bEndShotEffect;
	DWORD dwShotAnimElapsedTime;
	int iShotEffectSizeX = 43, iShotEffectSizeY = 40;
	int iShotCol;
	int iShotOffsetY;
};