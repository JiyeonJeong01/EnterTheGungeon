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
	virtual int Dir_AnimRow(Direction eDir) { return 0;  }
	void Shot_Bullet();

protected:
	CPlayer::PlayerState eState;

protected :
	Vector2 vInputDir;
	Direction eDir;
	POINT pInputCursor;
	DWORD dwLastFireTime;
	float fLimitFireTime;

	float fSpeed;
	bool bDodgePlaying;
	bool bCanShot;
};