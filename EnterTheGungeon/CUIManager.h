#pragma once
#include "CManager.h"
class CObject;
class CBoss;
class CInventory;
class CMouse;
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
	void Draw_Inventory(HDC hDC);
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

	// boss
	CBoss* pBoss;
	bool bBossDraw;
	DWORD dwEffecctTime;
	int iEffectIndex;
private:
	CMouse* pMouse;
};

