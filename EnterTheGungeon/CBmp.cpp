#include "pch.h"
#include "CBmp.h"

CBmp::CBmp()
{
	hMemDC = NULL;
	hCurBmp = NULL;
	hOldBmp = NULL;
}

CBmp::~CBmp()
{
	Release();
}

void CBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC hDC = GetDC(g_hWnd);

	hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	hCurBmp = (HBITMAP)LoadImage(NULL,
		pFilePath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	hOldBmp = (HBITMAP)SelectObject(hMemDC, hCurBmp);
}

void CBmp::Release()
{
	SelectObject(hMemDC, hOldBmp);
	DeleteDC(hMemDC);

	DeleteObject(hCurBmp);
}