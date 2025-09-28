#pragma once
#include "CMobState.h"

class CMobOwnerDeadState : public CMobState
{
public:
	CMobOwnerDeadState(CObject* pObj, CStateMachine* pStateMachine);
	~CMobOwnerDeadState() override {};

public:
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;

public:
	void Exit() override;
	void Enter() override;
	void Update_AnimFrame() override;
	void On_End_Animation() override;

private:
	int Dir_AnimRow(Direction eDir) override;
private:
	void Start_Dialogue();
	void Update_Dialogue();

private :
	bool bCanMoveAnim;
	int iDeadDir;;
	int iAnimRow;

private :
	bool bPlayDialogue;

	const TCHAR* szScript[2] = {
		L"내가 졌네, 항복이야!!",
		L"전부 가져가게..."
	};

	TCHAR szTmp[128] = { 0 };
	DWORD dwPressKeyElapsedTime;
	DWORD dwDialogueElapsedTime;

	int iScripteIndex;
	const int iMaxScriptIndex = 1;
	int iCurrentIndex;
	int iTotalLength;
};

