#pragma once
#include "CBossState.h"
class CBossKunaiAttackState : public CBossState
{
private:
	enum KunaiState { Setup, Dodge, Shot, Wait, KunaiEnd  };
public:
	CBossKunaiAttackState(CObject* pObj, CStateMachine* pStateMachine)
		: CBossState(pObj, pStateMachine) {
	}
	~CBossKunaiAttackState() override {};
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

private :
	void SettingUp();
	void Before_Dodge();
	void Dodging();
	void Shooting();
	void Waiting();

private :
	void Compute_DodgeDir();
	void Compute_DodgeAnimRow(Direction eDir);

private :
	KunaiState curState;
	const int iTotalAttack = 6;
	int iAttackCount;

	const TCHAR* animKey = L"";
private:
	// Set up
	const Vector2 vSetupPos = { 4800.f, 2800.f };

	// Dodge
	Vector2 vDodgeDestPos;
	Vector2 vDirToDest;

	const float fDodgeDist = 15.f;
	int iDodgeAnimTotalFrame = 8;
	int iDodgeAnimCol;
	int iDodgeAnimRow;
	bool bDodged[8];
	DWORD dwDodgeAnimTime;
	bool bDodgeTimer;

	// Shot
	const int iShotDirCount = 3;
	int iDegreeRange = 22;
	bool bDrawLine;
	Vector2 vShotDir[3];
	bool bCanShot;
	bool bShoot;
};

