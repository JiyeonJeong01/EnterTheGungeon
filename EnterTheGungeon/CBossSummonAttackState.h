#pragma once
#include "CBossState.h"

class CBossBullet;
class CBossSummonAttackState : public CBossState
{
private :
	enum SummonState { Creating, ShotWating, Shot, Summon, ReshotWating, ReShot, SummonEnd };
public:
	CBossSummonAttackState(CObject* pObj, CStateMachine* pStateMachine)
		: CBossState(pObj, pStateMachine) {
	}
	~CBossSummonAttackState() override {};
public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Exit() override;
	void Enter() override;

	void Update_AnimFrame() override;
	void Stop_Animation() override;
	void On_End_Animation() override;

public :
	void Create_Cheese();
	void Wait_Shot();
	void Summoning();
	void Reshot_Waiting();
	void ReShooting();

private:
	list<CBossBullet*> bulletList;
	list<Vector2> reshotDirList;
	RECT rCameraToRealPos;

	int iTotalBullet;
	int iCurBullet;

private :
	SummonState curState;
	DWORD dwWaitElapsedTime;
	float fWaitTime;
	RECT rBound;
	int iBombCurrIndex;
	DWORD dwReshotWaitingTime;
};