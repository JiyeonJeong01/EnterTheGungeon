#include "pch.h"
#include "CMobOwnerIdleState.h"
#include "CBmpManager.h"
#include "CInputManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"

CMobOwnerIdleState::CMobOwnerIdleState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{

}

void CMobOwnerIdleState::Initialize()
{
	CMobState::Initialize();

	fAttackTimeRange = 1.5f;
	dwLastAttackTime = GetTickCount();

	eState = CMob::Idle;
	animation.Initialize(0, 2, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 230);

	iCurrentIndex = iScripteIndex = 0;
}

void CMobOwnerIdleState::Update()
{
	CMobState::Update();

	Check_Dialogue();

	Update_AnimFrame();
}

void CMobOwnerIdleState::Late_Update()
{

}

void CMobOwnerIdleState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Owner");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 200;
	int iRenderSize = 200;

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		iRenderSize, iRenderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		iAnimRow * realSize,
		realSize, realSize,
		RGB(53, 53, 53));

	if (bCanDialogue)
	{
		if (dwPressKeyElapsedTime + 200 < GetTickCount())
		{
			dwPressKeyElapsedTime = GetTickCount();
			iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
		}
		int iRealSizeX = 32, iRealSizeY = 32;
		int iRenderSizeX = 40, iRenderSizeY = 40;

		HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"EKey");

		GdiTransparentBlt(hDC, renderer.Left() + 80, renderer.Top() + 51, iRenderSizeX, iRenderSizeY,
			hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
	}

	if (bPlayDialogue)
	{
		Update_Dialogue();
		int iRealSizeX = 660, iRealSizeY = 200;
		int iRenderSizeX = 220, iRenderSizeY = 67;
		HDC hDialogueDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Dialogue");

		GdiTransparentBlt(hDC, 	renderer.Left(), renderer.Top() - 10, 	iRenderSizeX, iRenderSizeY, 	
			hDialogueDC, 0, iRealSizeY * 2, 	iRealSizeX, iRealSizeY, RGB(53, 53, 53));

		// Text
		HFONT hFont = CreateFont( 	18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
		);

		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

		TCHAR buffer[64];
		swprintf_s(buffer, 64, szScript[iScripteIndex]);
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, renderer.Left() + 17, renderer.Top() + 3, buffer, lstrlen(szTmp));

		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
}

void CMobOwnerIdleState::Release()
{
}

void CMobOwnerIdleState::Exit()
{
}

void CMobOwnerIdleState::Enter()
{
	bPlayDialogue = bCanDialogue = false;
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	iScripteIndex = 0;
	dwPressKeyElapsedTime = GetTickCount();
	iAnimRow = 0;
}

void CMobOwnerIdleState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMobOwnerIdleState::Stop_Animation()
{
}

void CMobOwnerIdleState::On_End_Animation()
{
}

int CMobOwnerIdleState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() > 0) return 0;
	else return 1;
}

void CMobOwnerIdleState::Check_Dialogue()
{
	if (fCurDistToPlayer <= 170.f)
	{
		bCanDialogue = true;
		iAnimRow = 1;
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E') && !bPlayDialogue)
		{
			bCanDialogue = false;
			Start_Dialogue();
		}
	}
	else
	{
		bCanDialogue = false;
		iAnimRow = 0;
	}
}

void CMobOwnerIdleState::Start_Dialogue()
{
	iCurrentIndex = 0;
	iTotalLength = lstrlen(szScript[iScripteIndex]);

	dwDialogueElapsedTime = GetTickCount();
	fill(begin(szTmp), end(szTmp), 0);
	bPlayDialogue = true;
}

void CMobOwnerIdleState::Update_Dialogue()
{
	if (dwDialogueElapsedTime + 90 < GetTickCount())
	{
		dwDialogueElapsedTime = GetTickCount();
		if (iCurrentIndex < iTotalLength)
		{
			szTmp[iCurrentIndex] = szScript[iScripteIndex][iCurrentIndex];
			szTmp[iCurrentIndex + 1] = '\0';
			iCurrentIndex++;
		}
	}

	if (iCurrentIndex >= iTotalLength)
	{
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E') && iScripteIndex < iMaxScriptIndex)
		{
			iScripteIndex++;
			fill(begin(szTmp), end(szTmp), 0);
			iCurrentIndex = 0;
			iTotalLength = lstrlen(szScript[iScripteIndex]);
			dwDialogueElapsedTime = GetTickCount();
		}
		else if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E')  && iMaxScriptIndex >= iMaxScriptIndex)
		{
			bPlayDialogue = false;
		}
	}

}
