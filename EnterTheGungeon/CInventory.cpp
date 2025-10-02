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
	szCurItemKey = L"";
}

void CInventory::Initialize()
{
	iCartridge = iCoin = iKey = iBomb = 0;

	// 테스트용 코인 지급 
	iCoin = 500;
}

// CPlayer에서 호출하여 실행한다 
void CInventory::Update()
{
	if (pPlayer == nullptr) return;

	for (int i = 0; i < iTotalItem; ++i)
	{
		if (itemArray[i].first == L"Cartridge")
		{
			iCartridge = itemArray[i].second.size();
		}
		else if (itemArray[i].first == L"Bomb")
		{
			iBomb = itemArray[i].second.size();
		}
		else if (itemArray[i].first == L"Medkit")
		{
			iMedkit = itemArray[i].second.size();
		}
	}

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_SPACE) && curActiveItem != nullptr)
	{
		Try_UseItem(curActiveItem->Get_ItemKey());
	}

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('Q'))
	{
		Try_UseItem(L"Cartridge");
	}

}

void CInventory::Release()
{
	for (auto& p : itemArray)
	{
		//CRelease<list<CItem*>*>::Release(p.second);
	}
}

bool CInventory::Try_UseItem(const TCHAR* itemKey)
{

	auto iter = find_if(itemArray.begin(), itemArray.end(), [&](const pair< const TCHAR*, list<CItem*>>& p) -> bool {
		return lstrcmp(p.first, itemKey) == 0; // 같으면 0 반환
		});

	if (iter == itemArray.end())
	{
		return false;
	}

	// 아이템이 없는 경우
	if ((int)iter->second.size() == 0) 
		return false;

	// 맨 앞 아이템을 가져와서 
	auto& item = iter->second.front();
	item-> Apply_ItemEffect();

	// 사용 후 삭제
	// const TCHAR* itemKey = item->Get_ItemKey();
	iter->second.remove(item);
	if (iter->second.empty() &&(( itemKey == L"Bomb") || (itemKey == L"Medkit") || (itemKey == L"Boomerang")))
	{
		curActiveItem = nullptr; 
		szCurItemKey = nullptr;
	}
	return true;
}

void CInventory::Add_Item(const TCHAR* itemKey, CItem* pItem)
{
	// 같은 아이템의 리스트가 이미 존재하는지 확인
	auto iter = find_if(itemArray.begin(), itemArray.end(), [&](const pair<const TCHAR*, list<CItem*>>& p) -> bool {
		return (lstrcmp(p.first, itemKey) == 0);
		});

	// 없는 경우
	if (iter == itemArray.end())
	{
		int iNewIndex = -1;
		for (int i = 0; i < iTotalItem; ++i)
		{
			if (itemArray[i].first == nullptr)
				iNewIndex = i;
		}
		itemArray[iNewIndex].first = itemKey;
		itemArray[iNewIndex].second.push_back(pItem);
	}
	else
	{
		iter->second.push_back(pItem);
	}

	if (curActiveItem == nullptr && itemKey != L"Cartridge" && itemKey != L"Key" && itemKey != L"Coin")
	{
		curActiveItem = pItem;
		szCurItemKey = itemKey;
	}
}

void CInventory::Set_CurActiveItem(const TCHAR* itemKey)
{
	auto iter = find_if(itemArray.begin(), itemArray.end(), [&](const pair<const TCHAR*, list<CItem*>>& p) -> bool {
		return (lstrcmp(p.first, itemKey) == 0);
		});

	if (itemKey != L"Cartridge" && itemKey != L"Key" && itemKey != L"Coin")
	{
		curActiveItem = iter->second.front();
	}
}
