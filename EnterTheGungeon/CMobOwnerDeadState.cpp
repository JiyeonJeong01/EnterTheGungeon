#include "pch.h"
#include "CMobOwnerDeadState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"
#include "CStageManager.h"
#include "CItem.h"

CMobOwnerDeadState::CMobOwnerDeadState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMobOwnerDeadState::Initialize()
{
	CMobState::Initialize();

	fCurStateMaxTime = 60.f;
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Dead;

	animation.Initialize(0, 4, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 150);
}

void CMobOwnerDeadState::Update()
{
	CMobState::Update();

	if (bCanMoveAnim)
		Update_AnimFrame();
	else
	{
		animation.iCurrIndex = animation.iEndIndex;
	}

	if (dwCurrentStateElapsedTime + fCurStateMaxTime * 1000 < GetTickCount())
	{
		static_cast<CMob*>(pObj)->Set_Dead();
	}
}

void CMobOwnerDeadState::Render(HDC hDC)
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

	if (bPlayDialogue)
	{
		Update_Dialogue();
		int iRealSizeX = 660, iRealSizeY = 200;
		int iRenderSizeX = 220, iRenderSizeY = 67;
		HDC hDialogueDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Dialogue");

		GdiTransparentBlt(hDC, renderer.Left(), renderer.Top() + 50, iRenderSizeX, iRenderSizeY,
			hDialogueDC, 0, iRealSizeY * 2, iRealSizeX, iRealSizeY, RGB(53, 53, 53));

		// Text
		HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, HANGUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Galmuri9 Regular")
		);

		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

		TCHAR buffer[64];
		swprintf_s(buffer, 64, szScript[iScripteIndex]);
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, renderer.Left() + 17, renderer.Top() + 65, buffer, lstrlen(szTmp));

		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
	 
}

void CMobOwnerDeadState::Exit()
{
}


void CMobOwnerDeadState::Enter()
{
	bCanMoveAnim = true;
	CItem::Set_ItemFree();
	pObj->Get_Transform()->Direction({ 0.f, 0.f });
	dwCurrentStateElapsedTime = GetTickCount();
	MANAGER(CStageManager*, M_STAGE)->OnKilled_Enemy(static_cast<CMob*>(pObj));
	iAnimRow = 5;
	bPlayDialogue = false;
	iScripteIndex = 0;
}

void CMobOwnerDeadState::Update_AnimFrame()
{
	CState::Update_AnimFrame();

}

void CMobOwnerDeadState::On_End_Animation()
{
	animation.Initialize(0, 2, 0);
	iAnimRow = 6;
	if (!bPlayDialogue)
		Start_Dialogue();
}

int CMobOwnerDeadState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}

void CMobOwnerDeadState::Start_Dialogue()
{
	iCurrentIndex = 0;
	iTotalLength = lstrlen(szScript[iScripteIndex]);

	dwDialogueElapsedTime = GetTickCount();
	fill(begin(szTmp), end(szTmp), 0);
	bPlayDialogue = true;
}

void CMobOwnerDeadState::Update_Dialogue()
{
	if (dwDialogueElapsedTime + 90 < GetTickCount())
	{
		if (iCurrentIndex < iTotalLength)
		{
			dwDialogueElapsedTime = GetTickCount();
			szTmp[iCurrentIndex] = szScript[iScripteIndex][iCurrentIndex];
			szTmp[iCurrentIndex + 1] = '\0';
			iCurrentIndex++;
		}
	}

	if (iCurrentIndex >= iTotalLength && iScripteIndex < iMaxScriptIndex && dwDialogueElapsedTime +750 < GetTickCount())
	{
		iScripteIndex++;
		fill(begin(szTmp), end(szTmp), 0);
		iCurrentIndex = 0;
		iTotalLength = lstrlen(szScript[iScripteIndex]);
		dwDialogueElapsedTime = GetTickCount();
	}

}
