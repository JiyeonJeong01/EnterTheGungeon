#pragma once
#include "CObject.h"
class CPlayer;
class CBomb;
class CCartridge;
class CGuide :  public CObject
{
public :
	enum GuideState { Appear = 0, Left, Right, Smile };
	enum GuideStep { Move, Dodge, Shot, Reshot, Interact, ItemUse, Cartridge, SwapWeapon, End};

public :
	CGuide();
	~CGuide() override;

public :
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public :
	void Update_Transform() override;

private :
	void Detect_Player();
	void Update_Dialogue();
	void Update_AnimFrame();

private :
	void Guide_Move();
	void Guide_Dodge();
	void Guide_Shot();
	void Guide_Reshot();
	void Guide_Cartridge();
	void Guide_SwapWeapon();
	void Guide_ItemUse();
	void Guide_Interact();

private :
	void Check_NextScript();
	void Next_Script();

private :
	bool bPlayDialogue;
	bool bCanDialogue;
	bool bCanNextDialogue;
	bool bCompleteDialogue;
	bool bPressKey;

	const TCHAR* szScript[14] = {
		L"거기 잠깐!", // 0
		L"안녕, 내 이름은 세르 마누엘! ",  // 1
		L"모험을 시작하기 전에 알려줄 게 있어", // 2
		L"우선 가볍게 몸 좀 풀어볼까?", // 3
		L"W, A, S, D 키로 움직일 수 있어", // 4
		L"좋아, 이제 우클릭으로\n구르기를 해봐!!", // 5
		L"훌륭해!! 좌클릭으로\n총알을 발사해볼래?", // 6
		L"총알이 부족할 땐,\nR 키로 재장전할 수 있어", // 7
		L"E키를 눌러 아이템이나 테이블 같은\n물체와 상호작용 할 수 있어", //8
		L"Space 키로 아이템을 사용해봐!!", // 9
		L"Q 키로는 모든 총알을 삭제하는\n 카트리지를 사용할 수 있어",// 10
		L"Ctrl  키로 무기를 교체할 수 있는데\n이후에 시도해봐!",// 11
		L"좋아 이제 다 알려준 거 같은데...", //12
		L"너의 모험이\n성공적으로 끝나길 바랄게!!" // 13
	};

	TCHAR szTmp[64] = { 0 };
	DWORD dwAnimFrameElapsedTime;
	DWORD dwPressKeyAnimElapsedTime;
	DWORD dwDialogueElaspedTime;
	DWORD dwStepElapsedTime;

	int iPressKeyAnimCol;
	int iCurScriptIndex;
	const int iTotalScriptIndex = 13;

	int iCurLetterIndex;
	int iTotalLength;

	float fDialogueRange;

private :
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


private :
	CPlayer* pPlayer;
	GuideState eGuideState;
	GuideStep eGuideStep;

private :
	float fSpeed;

private :
	bool bMoveCompleted, bDodgeCompleted, bShotCompleted, bReshotCompleted, bCartridgeCompleted, bSwapWeaponCompleted, bInteractCompleted, bItemUseCompleted;
	bool bCreateItem,  bCreateBullet, bUsedItem;

private:
	DWORD dwAttackTime;
	float fAttackRange;
	CBomb* pBomb;
	CCartridge* pCartridge;
	int iBombFallCount;

private :
	bool bGuideMove[4] = { false, };
	int iCount;
	const int iMaxCount = 200;
	const TCHAR* spriteKey;

};

