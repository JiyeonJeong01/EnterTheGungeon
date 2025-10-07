#pragma once
#include "CObject.h"
class CEndingGuide :  public CObject
{
public :
	enum EndingGuideState { Appear = 0, Left, Right, Smile };
public:
	CEndingGuide();
	~CEndingGuide() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void Should_Dialogue();
	void Start_Dialogue();
	void Update_Transform() override;
	void Update_Dialogue();
	void Update_AnimFrame();

private :
	bool bPlayDialogue;
	bool bCanDialogue;
	bool bCanNextDialogue;
	bool bPressKey;
	bool bCompleteDialogue;

private :
	const TCHAR* szScript[4] = {
	L"!!!!!!!!!!", // 0
	L"자네, 해낼 줄 알았다네!!! ",  // 1
	L"정말 고맙네, 고마워!", // 2
	L"선물을 준비했는데...\n 위 방으로 가보게나", // 3
	};

	TCHAR szTmp[64] = { 0 };
	DWORD dwAnimFrameElapsedTime;
	DWORD dwPressKeyAnimElapsedTime;
	DWORD dwDialogueElaspedTime;

	int iPressKeyAnimCol;
	int iCurScriptIndex;
	const int iTotalScriptIndex = 3;

	int iCurLetterIndex;
	int iTotalLength;

	float fDialogueRange;

private:
	// Animation
	const int iRealSizeX = 80;
	const int iRealSizeY = 80;
	int iRenderSizeX, iRenderSizeY;
	int iAnimRow, iAnimCol, iMaxAnimCol;
	int iDialogueOffsetX, iDialogueOffsetY;
	float fFrameRange;

	const int iPanelRealSizeX = 660;
	const int iPanelRealSizeY = 200;
	int iPanelRenderSizeX, iPanelRenderSizeY;
	int iPanelOffsetX, iPanelOffsetY;
	int iAnimRange;
	const int iPanelOffsetX03 = -70, iPanelOffesetY03 = -90;

private:
	EndingGuideState eGuideState;
	int prevScriptIndex;

};

