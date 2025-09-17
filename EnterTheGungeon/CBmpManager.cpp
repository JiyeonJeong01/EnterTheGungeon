#include "pch.h"
#include "CBmpManager.h"
#include "CRelease.h"
#include "CSTLHelper.h"

CBmpManager::CBmpManager()
{
}

CBmpManager::~CBmpManager()
{
	Release();
}

void CBmpManager::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = find_if(mBmpMap.begin(), mBmpMap.end(), [&](const auto& p)->bool {
		return CSTLHelper::Compare_Key(pImgKey, p);
		});

	if (iter == mBmpMap.end())
	{
		CBmp* pBmp = new CBmp;

		pBmp->Load_Bmp(pFilePath);
		mBmpMap.insert({ pImgKey, pBmp });
	}
}

HDC CBmpManager::Find_Image(const TCHAR* pImgKey)
{
	auto iter = find_if(mBmpMap.begin(), mBmpMap.end(), [&](const auto& p)->bool { 
		return CSTLHelper::Compare_Key(pImgKey, p); 
		});

	if (iter == mBmpMap.end()) 
		return nullptr;
	return iter->second->Get_MemDC();
}

void CBmpManager::Release()
{
	for (auto& bmp : mBmpMap)
	{
		CSTLHelper::Delete_Map(bmp);
	}
	mBmpMap.clear();
}
