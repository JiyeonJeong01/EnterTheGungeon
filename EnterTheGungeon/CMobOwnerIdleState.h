#pragma once
#include "CMobState.h"

class CObject;
class CStateMachine;

class CMobOwnerIdleState : public CMobState
{
public:
	CMobOwnerIdleState(CObject* pObj, CStateMachine* pStateMachine);
	~CMobOwnerIdleState() override {};

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

private :
	void Check_Dialogue();
	void Start_Dialogue();
	void Update_Dialogue();

private :
	bool bPlayDialogue;
	bool bCanDialogue;

	const TCHAR* szScript[2] = {
		L"¹¹, µ·ÀÌ ºÎÁ·ÇÏ´Ù°í...?",
		L"ÀÌºÁ ±ð¾ÆÁÙ ¼ö´Â ¾ø¾î!!"
	};

	TCHAR szTmp[128] = { 0 };
	DWORD dwPressKeyElapsedTime;
	DWORD dwDialogueElapsedTime;

	int iPressKeyAnimCol;

	int iScripteIndex;
	const int iMaxScriptIndex = 1;
	int iCurrentIndex;
	int iTotalLength;

	int iAnimRow;
};

