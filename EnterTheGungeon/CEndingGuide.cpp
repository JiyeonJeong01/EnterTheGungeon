#include "pch.h"
#include "CEndingGuide.h"
#include "pch.h"
#include "CGuide.h"
#include "CBmpManager.h"
#include "CInputManager.h"
#include "CObject.h"
#include "CObjectFactory.h"
#include "CBomb.h"
#include "CCartridge.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMobBullet.h"
#include "CSoundManager.h"

CEndingGuide::CEndingGuide()
{
	Release();
}

CEndingGuide::~CEndingGuide()
{
	Release();
}

void CEndingGuide::Initialize()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Player/Guide.bmp", L"GuideNPC");

	CObject::Initialize();

	pRenderer->rType = RND__GAMEBOJECT;
	eType = O_ENEMY;

	bCanDialogue = false;
	bPlayDialogue = false;
	bCompleteDialogue = false;
	bPressKey = false;
	fDialogueRange = 0.08f;

	iMaxAnimCol = 8;
	iAnimRow = iAnimCol = iCurLetterIndex = iCurScriptIndex = 0;
	iRenderSizeX = iRenderSizeY = iRealSizeX;
	fFrameRange = 0.1f;
	dwAnimFrameElapsedTime = GetTickCount();
	dwPressKeyAnimElapsedTime = GetTickCount();
	eGuideState = CEndingGuide::Appear;

	iPanelOffsetX = iPanelOffsetX03;
	iPanelOffsetY = iPanelOffesetY03;
	iPanelRenderSizeX = 330;
	iPanelRenderSizeY = 90;

	prevScriptIndex = 0;

}

int CEndingGuide::Update()
{
	Should_Dialogue();
	if (prevScriptIndex != iCurScriptIndex)
	{
		MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"sfx_system_voice.mp3", 1.f);
		prevScriptIndex = iCurScriptIndex;
	}
	if (bPlayDialogue)
		Update_Dialogue();

	return 0;
}

void CEndingGuide::Late_Update()
{
	CObject::Update_Renderer();
	Update_AnimFrame();
}

void CEndingGuide::Render(HDC hDC)
{
#pragma region NPC
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"GuideNPC");

	RECT rBound = pRenderer->Get_Rect();

	GdiTransparentBlt(hDC,
		rBound.left, rBound.top,
		iRenderSizeX, iRenderSizeY,
		hMemDC,
		iAnimCol * iRealSizeX,
		iAnimRow * iRealSizeY,
		iRealSizeX, iRealSizeY,
		RGB(56, 56, 56));
#pragma endregion

#pragma region Dialogue
	if (bPlayDialogue)
	{
		Update_Dialogue();

		HDC hDialogueDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Dialogue");

		GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY, iPanelRenderSizeX, iPanelRenderSizeY,
			hDialogueDC, 0, iPanelRealSizeY * 2, iPanelRealSizeX, iPanelRealSizeY, RGB(53, 53, 53));

		//// Text
		HFONT hFont = CreateFont(
			20, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE,
			HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			_T("Galmuri9 Regular")
		);

		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

		RECT rcText = { rBound.left + iPanelOffsetX + 20,	rBound.top + iPanelOffsetY + 10,
									rBound.left + iPanelRenderSizeX - 20, rBound.top + iPanelRenderSizeY - 10 };

		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		DrawText(hDC, szTmp, -1, &rcText, DT_LEFT | DT_TOP | DT_WORDBREAK);
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
#pragma endregion

#pragma region Press Key
	if (bPlayDialogue && bPressKey && iCurScriptIndex != iTotalScriptIndex)
	{
		if (dwPressKeyAnimElapsedTime + iAnimRange < GetTickCount())
		{
			dwPressKeyAnimElapsedTime = GetTickCount();
			iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
		}
		int iRealSizeX = 32, iRealSizeY = 32;
		int iRenderSizeX = 40, iRenderSizeY = 40;

		HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"EKey");
		GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
			hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
	}
#pragma endregion
}

void CEndingGuide::Release()
{
}

void CEndingGuide::Update_Transform()
{

}

void CEndingGuide::Should_Dialogue()
{
	bPressKey = true;
	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E') && !bPlayDialogue)
	{
		Start_Dialogue();
	}
}

void CEndingGuide::Start_Dialogue()
{
	iCurScriptIndex = 0;
	iTotalLength = lstrlen(szScript[iCurScriptIndex]);

	dwDialogueElaspedTime = GetTickCount();
	fill(begin(szTmp), end(szTmp), 0);
	bPlayDialogue = true;
}

void CEndingGuide::Update_Dialogue()
{
	if (dwDialogueElaspedTime + fDialogueRange * 1000 < GetTickCount())
	{
		dwDialogueElaspedTime = GetTickCount();
		if (iCurLetterIndex < iTotalLength)
		{
			szTmp[iCurLetterIndex] = szScript[iCurScriptIndex][iCurLetterIndex];
			szTmp[iCurLetterIndex + 1] = '\0';
			iCurLetterIndex++;
			bCanNextDialogue = false;
		}
	}

	// 모든 문자를 출력한 경우
	if (iCurLetterIndex >= iTotalLength)
	{
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E') && iCurScriptIndex < iTotalScriptIndex)
		{
			iCurScriptIndex++;
			fill(begin(szTmp), end(szTmp), 0);
			iCurLetterIndex = 0;
			iTotalLength = lstrlen(szScript[iCurScriptIndex]);
			dwDialogueElaspedTime = GetTickCount();
		}
		else if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E') && iCurScriptIndex >= iTotalScriptIndex)
		{
			bPlayDialogue = false;
		}
	}
}

void CEndingGuide::Update_AnimFrame()
{
	if (dwAnimFrameElapsedTime + fFrameRange * 1000 < GetTickCount())
	{
		dwAnimFrameElapsedTime = GetTickCount();
		iAnimCol++;
		if (iAnimCol > iMaxAnimCol)
		{
			if (iAnimRow == 0)
			{
				iAnimRow = 3;
				iMaxAnimCol = 1;
				fFrameRange = 0.2f;
			}
			iAnimCol = 0;
		}
	}
}
