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

protected:
	CPlayer::PlayerState eState;

protected :
	Vector2 vInputDir;
	POINT pInputCursor;
	DWORD dwLastFireTime;

	float fSpeed;
	bool bDodgePlaying;
	Direction eDir;
};