#include "pch.h"
#include "CInventory.h"
#include "CItem.h"
#include "CSTLHelper.h"
#include "CBmpManager.h"
#include "CInputManager.h"
#include "CRelease.h"

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	iCartridge = iCoin = iKey = iBomb = 0;
	// for test
	iCoin = 500;
}

// CPlayer에서 호출하여 실행한다 
void CInventory::Update()
{
	if (pPlayer == nullptr) return;

	auto iter = find_if(itemMap.begin(), itemMap.end(), [&](const auto& p) -> bool { return CSTLHelper::Compare_Key(L"Cartridge", p); });
	iCartridge = (iter == itemMap.end()) ? 0 : (int)iter->second->size();

	iter = find_if(itemMap.begin(), itemMap.end(), [&](const auto& p) -> bool { 	return CSTLHelper::Compare_Key(L"Key", p); });
	iKey = (iter == itemMap.end()) ? 0 : (int)iter->second->size();

	iter = find_if(itemMap.begin(), itemMap.end(), [&](const auto& p) -> bool { 	return CSTLHelper::Compare_Key(L"Bomb", p); });
	iBomb = (iter == itemMap.end()) ? 0 : (int)iter->second->size();

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_SPACE) && curActiveItem != nullptr)
	{
		Try_UseItem(curActiveItem->Get_ItemKey());
	}

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('Q'))
	{
		Try_UseItem(L"Cartridge");
	}

	//printf("cartridge : %d\n", iCartridge);
	//printf("key : %d\n", iKey);
	//printf("bomb : %d\n", iBomb);
	//printf("coin : %d\n", iCoin);
}

void CInventory::Release()
{
	for (auto& p : itemMap)
	{
		CRelease<list<CItem*>*>::Release(p.second);
	}
}

bool CInventory::Try_UseItem(const TCHAR* itemKey)
{

	auto iter = find_if(itemMap.begin(), itemMap.end(), [&](const auto& p) -> bool {
		return CSTLHelper::Compare_Key(itemKey, p);
		});

	if (iter == itemMap.end())
	{
		return false;
	}
	else
	{
		if ((int)iter->second->size() == 0) 
			return false;
		auto& item = iter->second->front();
		item-> Apply_ItemEffect();
		const TCHAR* itemKey = item->Get_ItemKey();
		iter->second->remove(item);
		if (iter->second->empty() &&( itemKey == L"Bomb") && (itemKey == L"Medkit"))
		{
			curActiveItem = nullptr;
		}
	}
	return true;
}

void CInventory::Add_Item(const TCHAR* itemKey, CItem* pItem)
{
	auto iter = find_if(itemMap.begin(), itemMap.end(), [&](const auto& p) -> bool {
		return CSTLHelper::Compare_Key(itemKey, p);
		});

	if (iter == itemMap.end())
	{
		list<CItem*>* pList = new list<CItem*>();
		pList->push_back(pItem);
		itemMap.insert({ itemKey, pList});
	}
	else
	{
		iter->second->push_back(pItem);
	}

	if (curActiveItem == nullptr && itemKey != L"Cartridge" && L"Key" && L"Coin")
	{
		curActiveItem = pItem;
	}

}

void CInventory::Draw_Inventory(HDC hDC)
{
}
