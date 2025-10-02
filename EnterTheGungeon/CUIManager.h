#pragma once
#include "CManager.h"
class CObject;
class CBoss;
class CInventory;
class CMouse;
class CItem;

class CUIManager :  public CManager
{
public :
	CUIManager();
	~CUIManager() override;

public :
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC);
	void Release() override;

public:
	void Add_Object(CObject* pObj);
	bool Remove_Object(ObjectType eObject, CObject* pObj);
	list<CObject*>* Get_Object(ObjectType eObject);

public :
	void Draw_BasicInventory(HDC hDC);
	void Draw_ClosedInventory(HDC hDC);
	void Draw_OpenedInventory(HDC hDC);

	void Draw_CartridgeEffect(HDC hDC);
	void Start_CartridgeEffect();

private :
	void Prepare_OpenInventory();


public :
	void Set_WheelScroll(int iDelta) { iScroll = iDelta; }


	void Draw_BossStat(HDC hDC);
	CMouse* Get_Mouse() { return pMouse;  }

public :
	list<CObject*> uiObjects;

	CInventory* pInventory;

	// inventory
	bool bDrawPlayer;
	int iCartridge;
	int iCoin;
	int iKey;
	int iBomb;

	// Open Inventory
	bool bOpenInventory;
	DWORD dwLastWheelElapsedTime;
	int iWheelRange;
	int iItemTypeCount;
	map<const TCHAR*, list<CItem*>> displayInventory;
	map<const TCHAR*, list<CItem*>>::iterator curDisplayIter;
	map<const TCHAR*, list<CItem*>>::iterator baseDisplayIter;
	bool bActivatedSlotTurn;

	// boss
	CBoss* pBoss;
	bool bBossDraw;
	DWORD dwEffecctTime;
	int iEffectIndex;
private:
	CMouse* pMouse;
	int iScroll;


private :
	bool bShouldCartridgeAnim;
	bool bEndCartridgeAnim;
	DWORD dwCartridgeAnim;
	int iCartridgeCol, iCartridgeRow;
	int iCartridgeColMax = 5;
	int iCartridgeSize = 300;

};

