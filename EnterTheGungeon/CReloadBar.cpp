#include "pch.h"
#include "CReloadBar.h"
#include "CBmpManager.h"
#include "CPlayer.h"
#include "CCameraManager.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CSoundManager.h"

CReloadBar::CReloadBar()
{
}

CReloadBar::~CReloadBar()
{
	Release();
}

void CReloadBar::Initialize()
{
	CObject::Initialize();

	bActive = true;
	pRenderer->rType = RND__UI;


	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/ReloadBar.bmp", L"ReloadBar");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/ReloadScroll.bmp", L"ReloadScroll");

	pTransform->Size({ 98.f, 17.f });
	pCollider->Size({ 98.f, 17.f });
	pRenderer->Size({ 98.f, 17.f });

	bActive = false;
	fReloadTime = 1.5f;
	iCurMoveDist = 0;
	iMoveDistPerFrame = 1;
}

int CReloadBar::Update()
{
	if (bActive)
	{
		iCurMoveDist += iMoveDistPerFrame;
		if (iCurMoveDist >= pRenderer->Size().X() - 10)
		{
			iMoveDistPerFrame *= -1;
		}
		if (iCurMoveDist < 0)
		{
			iCurMoveDist = 0;
			iMoveDistPerFrame *= -1;
			bActive = false;
			pPlayer->Finist_Reloading();
		}
	}
	return 0;

}

void CReloadBar::Late_Update()
{
}

void CReloadBar::Render(HDC hDC)
{
	if (!bActive) return;
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"ReloadBar");
	HDC hMemDC2 = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"ReloadScroll");

	RECT rRender = pPlayer->Get_Renderer()->Get_Rect();

	int iLeft = (rRender.left + pPlayer->Get_Renderer()->Size().X() * 0.5f) - pRenderer->Size().X() * 0.5f;
	int iTop = rRender.top -40;

	GdiTransparentBlt(hDC,
		iLeft, iTop,
		98, 17,
		hMemDC,
		0, 0, 
		98, 17, 
		RGB(255, 0, 255));

	GdiTransparentBlt(hDC,
		iLeft + iCurMoveDist, iTop,
		7, 18,
		hMemDC2,
		0, 0,
		7, 18,
		RGB(255, 0, 255));

		// Text
		HFONT hFont = CreateFont(
			18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
		);
	
		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
	
		TCHAR buffer[64];
		swprintf_s(buffer, 64, _T("ÀçÀåÀü !!"));
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, iLeft + 20, iTop - 25, buffer, lstrlen(buffer));
	
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
}

void CReloadBar::Release()
{
}

void CReloadBar::Update_Renderer()
{
}

void CReloadBar::Start_Reload()
{
	if (bActive == false)
	{
		bActive = true;
		dwTime = GetTickCount();
		MANAGER(CSoundManager*, M_SOUND)->StopSound(SOUND_EFFECT);
		MANAGER(CSoundManager*, M_SOUND)->PlaySoundW(L"Player_Reload.wav", SOUND_EFFECT, 1.f);
	}
}
