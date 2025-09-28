#pragma once
class CPlayer;
class CItem;
class CInventory
{
public :
	CInventory(CPlayer* pPlayer = nullptr) : pPlayer(pPlayer), curActiveItem(nullptr) {}
	~CInventory();

public :
	void Initialize();
	void Update();
	void Release();

public :
	bool Try_UseItem(const TCHAR* itemKey);
	void Add_Item(const TCHAR* itemKey, CItem* pItem);

public :
	void Draw_Inventory(HDC hDC);

public :
	int Get_Cartridge() const { return iCartridge; }
	void Set_Cartridge(int count) { iCartridge = count; }

	int Get_Coin() const { return iCoin; }
	void Set_Coin(int count) { iCoin = count; }
	void Minus_Coin(int iValue) { iCoin = (iCoin - iValue >= 0) ? iCoin - iValue : 0; }

	int Get_Key() const { return iKey; }
	void Set_Key(int count) { iKey = count; }

	CItem* Get_CurActiveItem() { return curActiveItem;  }

	map<const TCHAR*, list <CItem*>*>* Get_ItemMap() { return &itemMap; }

private :
	CPlayer* pPlayer;

	int iCartridge;
	int iCoin;
	int iKey;
	int iBomb;

private :
	map<const TCHAR*, list<CItem*>*> itemMap;
	CItem* curActiveItem;
};

