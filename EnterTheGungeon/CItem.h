#pragma once
#include "CObject.h"
#include "CPlayer.h"
#include "CInventory.h"
class CItem : public CObject
{
public :
	CItem() {}
	~CItem() override;
public:
	static void Load_Resource();

public :
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public :
	void Detect_Player();
	virtual void Late_Initialize();
	virtual void Drop_Item(Vector2 vDropPos);
	virtual void Get_Item();
	virtual void Apply_ItemEffect();

public :
	virtual void Display_ItemInfo(HDC hDC) {};
	virtual void OnDetect_PlayerIn();
	virtual void OnDetect_PlayerOut();

public : 
	bool Get_CanUse() { return bCanUse;  }
	const RECT& Get_DetectBound() { return rDetectBound; }
	bool Get_Obtained() { return bObtained; }

	const TCHAR* Get_ItemKey() { return spriteKey;  }
	void Set_ForSell(bool bSell) { bForSell = bSell; }

	void Show_EKey(HDC hDC);
	void Show_Guide_Success(HDC hDC);
	void Show_Guide_Fail(HDC hDC);

public :
	void Set_Dead() { bAlive = false; }

protected:
	CPlayer* pPlayer;
	RECT rDetectBound;

	bool bForSell;
	bool bDisplayInfo;
	int iPrice;
	bool bDisplayPopup;
	bool bDisplaySuccessPopup;
	bool bDisplayFailPopup;
	bool bDisplayPressE;

	DWORD dwPressKeyElapsedTime;
	int iPressKeyAnimCol;

	bool bCanUse;
	bool bCanInteract;
	bool bObtained;

	int iAnimRow;
	int iAnimCol;
	int iAnimSizeX;
	int iAnimSizeY;

	int iRealSizeX;
	int iRealSizeY;
	bool bRealSize;

	const TCHAR* spriteKey;
	const int iInfoRealSizeX = 242, iInfoRealSizeY = 150;
	const int iPopupRealSizeX = 660, iPopupRealSizeY = 200;

public :
	static void Set_ItemFree() { bFree = true; }
	static bool Get_IemFree() { return bFree;  }
protected:
	static DWORD dwLastPurchasedTime;
	static bool bFree;

};

