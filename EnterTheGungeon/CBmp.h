#pragma once
class CBmp
{
public :
	CBmp();
	~CBmp();

public :
	HDC Get_MemDC() { return hMemDC;  }
	void Load_Bmp(const TCHAR* pFilePath);
	void Release();

private:
	HDC hMemDC;
	HBITMAP hCurBmp;
	HBITMAP hOldBmp;
};

