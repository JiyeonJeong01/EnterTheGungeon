#pragma once
#include "CMobState.h"
class CMob02KnockbackState :  public CMobState
{
public:
	CMob02KnockbackState(CObject* pObj, CStateMachine* pStateMachine);
	~CMob02KnockbackState() override {};

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
	int Get_WeaponPos();
	int Get_WeaponDir();

};

