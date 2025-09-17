#pragma once
#include "CManager.h"
#include "CBmp.h"
class CBmpManager : public CManager
{
public :
	CBmpManager();
	~CBmpManager() override;

public :
	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC Find_Image(const TCHAR* pImgKey);
	void Release();

private :
	map<const TCHAR*, CBmp*> mBmpMap;
};

