#pragma once
#include "CBossState.h"
class CBossDodgeState :   public CBossState
{
public:
	CBossDodgeState(CObject* pObj, CStateMachine* pStateMachine)
		: CBossState(pObj, pStateMachine) {
	}
	~CBossDodgeState() override {};
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

	int Dir_AnimRow(Direction eDir) override;

public :
	void Set_ForceDodge() { bForceDodge = true; }
private: 
	void Dodge();

private :
	int iCurDodge;
	CAnimation effectAnim;
	bool bEffectPlay;
	POINT dodgeEffectPos;
	bool bForceDodge;
};

