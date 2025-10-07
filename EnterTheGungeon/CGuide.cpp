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

CGuide::CGuide()
{
	pPlayer = nullptr;
}

CGuide::~CGuide()
{
	Release();
}

void CGuide::Initialize()
{
	CObject::Initialize();

	pRenderer->rType = RND__GAMEBOJECT;
	eType = O_ENEMY;

	bCanDialogue = false;
	bPlayDialogue = false;
	bCompleteDialogue = false;
	bPressKey = false;
	fDialogueRange = 0.008f;

	iMaxAnimCol = 8;
	iAnimRow = iAnimCol = iCurLetterIndex = iCurScriptIndex = 0;
	iRenderSizeX = iRenderSizeY = iRealSizeX;
	fFrameRange = 0.1f;
	dwAnimFrameElapsedTime = GetTickCount();
	dwPressKeyAnimElapsedTime = GetTickCount();
	eGuideState = GuideState::Appear;
	eGuideStep = GuideStep::Move;

	fSpeed = 0.f;

	prevScriptIndex = 0;

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Player/Guide.bmp", L"GuideNPC");

	iPanelOffsetX = iPanelOffsetX03;
	iPanelOffsetY = iPanelOffesetY03;
	iPanelRenderSizeX = 330; 
	iPanelRenderSizeY = 90;

	bMoveCompleted = bDodgeCompleted = bShotCompleted = bReshotCompleted = bCartridgeCompleted = 
		bSwapWeaponCompleted = bInteractCompleted = bItemUseCompleted =false;
	bCreateItem = bCreateBullet = false;
	iCount = 0;
	iPressKeyAnimCol = 0;
	spriteKey = L"EKey";

	iAnimRange = 300;
	iBombFallCount = 0;
	bUsedItem = false;
}

int CGuide::Update()
{
	if (prevScriptIndex != iCurScriptIndex)
	{
		MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"sfx_system_voice.mp3", 1.f);
		// MANAGER(CSoundManager*, M_SOUND)->PlaySoundW(L"sfx_system_voice.mp3", SOUND_EFFECT, 1.f);
		prevScriptIndex = iCurScriptIndex;
	}


	if (!bAlive) return ObjectState::S_DEAD;
	switch (eGuideStep)
	{
	case CGuide::Move:
		Guide_Move();
		break;
	case CGuide::Dodge:
		Guide_Dodge();
		break;
	case CGuide::Shot:
		Guide_Shot();
		break;
	case CGuide::Reshot:
		Guide_Reshot();
		break;
	case CGuide::Cartridge:
		Guide_Cartridge();
		break;
	case CGuide::SwapWeapon:
		Guide_SwapWeapon();
		break;
	case CGuide::ItemUse:
		Guide_ItemUse();
		break;
	case CGuide::Interact:
		Guide_Interact();
		break;
	default:
		break;
	}

	Check_NextScript();


	return 0;
}

void CGuide::Late_Update()
{
	CObject::Update_Renderer();
	Update_AnimFrame();
}

void CGuide::Render(HDC hDC)
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

		RECT rcText = {	rBound.left + iPanelOffsetX + 20,	rBound.top + iPanelOffsetY + 10,
									rBound.left + iPanelRenderSizeX - 20, rBound.top + iPanelRenderSizeY - 10};

		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		DrawText(hDC, szTmp, -1, &rcText, DT_LEFT | DT_TOP | DT_WORDBREAK);
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);
	}
#pragma endregion

#pragma region Press Key
	if (bPlayDialogue && bPressKey && iCurScriptIndex != 13)
	{
		if (iCurScriptIndex == 4)
		{
			if (dwPressKeyAnimElapsedTime + iAnimRange < GetTickCount())
			{
				dwPressKeyAnimElapsedTime = GetTickCount();
				iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
			}
			int iRealSizeX = 32, iRealSizeY = 32;
			int iRenderSizeX = 40, iRenderSizeY = 40;

			HDC wKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"WKey");
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				wKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));

			HDC aKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"AKey");
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX + iRenderSizeX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				aKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));

			HDC sKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"SKey");
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX + iRenderSizeX * 2, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				sKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));

			HDC dKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"DKey");
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX + iRenderSizeX * 3, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				dKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
		} // WASD
		else if (iCurScriptIndex == 5 || iCurScriptIndex == 6) // 우클릭, 좌클릭
		{
			if (iCurScriptIndex == 5) spriteKey = L"RightClick";
			if (iCurScriptIndex == 6) spriteKey = L"LeftClick";

			if (dwPressKeyAnimElapsedTime + iAnimRange < GetTickCount())
			{
				dwPressKeyAnimElapsedTime = GetTickCount();
				iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
			}
			int iRealSizeX = 60, iRealSizeY = 40;
			int iRenderSizeX = 60, iRenderSizeY = 40;

			HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));

		}
		else if (iCurScriptIndex == 11)
		{
			if (dwPressKeyAnimElapsedTime + iAnimRange < GetTickCount())
			{
				dwPressKeyAnimElapsedTime = GetTickCount();
				iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
			}
			int iRealSizeX = 48, iRealSizeY = 32;
			int iRenderSizeX = 60, iRenderSizeY = 40;

			HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"CtrlKey");
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
		}
		else if (iCurScriptIndex == 9)
		{
			if (dwPressKeyAnimElapsedTime + iAnimRange < GetTickCount())
			{
				dwPressKeyAnimElapsedTime = GetTickCount();
				iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
			}
			int iRealSizeX = 64, iRealSizeY = 32;
			int iRenderSizeX = 64, iRenderSizeY = 40;

			HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"SpaceKey");
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
		}
		else
		{
			if (dwPressKeyAnimElapsedTime + iAnimRange < GetTickCount())
			{
				dwPressKeyAnimElapsedTime = GetTickCount();
				iPressKeyAnimCol = ++iPressKeyAnimCol % 2;
			}
			int iRealSizeX = 32, iRealSizeY = 32;
			int iRenderSizeX = 40, iRenderSizeY = 40;

			HDC hKeyDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);
			GdiTransparentBlt(hDC, rBound.left + iPanelOffsetX, rBound.top + iPanelOffsetY - 40, iRenderSizeX, iRenderSizeY,
				hKeyDC, iPressKeyAnimCol * iRealSizeX, 0, iRealSizeX, iRealSizeY, RGB(0, 0, 0));
		}

	}

#pragma endregion
}

void CGuide::Release()
{

}

void CGuide::Update_Transform()
{
	Vector2 vNew = { pTransform->Position().X() + pTransform->Direction().X() * fSpeed,
						 pTransform->Position().Y() + pTransform->Direction().Y() * fSpeed };
	pTransform->Position(move(vNew));
}

void CGuide::Detect_Player()
{

}

void CGuide::Update_Dialogue()
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
		if (iCurLetterIndex >= iTotalLength && iCurScriptIndex == 13 && iAnimRow != 3)
		{
			bCompleteDialogue = true;
			// bPlayDialogue = false;
			//bCanDialogue = false;
			bPressKey = false;
			bCanNextDialogue = false;
			iAnimRow = 3;
			iMaxAnimCol = 1;
			fFrameRange = 0.25;
			iAnimCol = 0;
		}
	}

	// 모든 문자를 출력한 경우
	if (iCurLetterIndex >= iTotalLength)
	{
		// 0~3 번 대화는 조건을 필요로하지 않는다
		if (iCurScriptIndex == 3)
		{
			fill(begin(bGuideMove), end(bGuideMove), false);
		}
		if (iCurScriptIndex == 0 || iCurScriptIndex == 1 || iCurScriptIndex == 2 || iCurScriptIndex == 3
				|| iCurScriptIndex == 12 || iCurScriptIndex == 13)
		{
			bPressKey = true;
			if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E') && iCurScriptIndex < iTotalScriptIndex)
				bCanNextDialogue = true;
			else
				bCanNextDialogue = false;
		}
		else
		{
			bCanNextDialogue = false;
		}
		// 다음 대화로 전환이 가능한 경우
		if (bCanNextDialogue)
		{
			iCurLetterIndex = 0;
			iCurScriptIndex++;
			iTotalLength = lstrlen(szScript[iCurScriptIndex]);
			dwDialogueElaspedTime = GetTickCount();
			fill(begin(szTmp), end(szTmp), 0);
		}
	}
}

void CGuide::Update_AnimFrame()
{
	if (dwAnimFrameElapsedTime + fFrameRange * 1000 < GetTickCount())
	{
		dwAnimFrameElapsedTime = GetTickCount();
		iAnimCol++;
		if (iAnimCol > iMaxAnimCol)
		{
			if (iAnimRow == GuideState::Appear)
			{
				iAnimRow = GuideState::Left;
				eGuideState = GuideState::Left;
				iMaxAnimCol = 5;
				bPressKey = true;
			}
			iAnimCol = 0;
		}
	}
}

void CGuide::Guide_Move()
{
	if (!bCanNextDialogue)
	{
		if (!bGuideMove[0])
			bGuideMove[0] = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('W');
		if (!bGuideMove[1])
			bGuideMove[1] = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('A');
		if (!bGuideMove[2])
			bGuideMove[2] = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('S');
		if (!bGuideMove[3])
			bGuideMove[3] = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('D');

		bool bClearStep = true;
		for (int i = 0; i < 4; i++)
		{
			if (!bGuideMove[i])
			{
				bClearStep = false;
				break;
			}
		}
		bCanNextDialogue = bClearStep;
		bMoveCompleted = bCanNextDialogue;
	}
}

void CGuide::Guide_Dodge()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RBUTTON);
		iCount++;
		if (iCount >= iMaxCount)
			bDodgeCompleted = bCanNextDialogue;
	}
}

void CGuide::Guide_Shot()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_LBUTTON);
		iCount++;
		if (iCount >= iMaxCount)
			bShotCompleted = true;
	}
}

void CGuide::Guide_Reshot()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('R');
		bReshotCompleted = true;
	}
}

void CGuide::Guide_Cartridge()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('Q');
		iCount++;
		if (iCount >= iMaxCount)
			bCartridgeCompleted = true;
	}

	if (dwAttackTime + 700 < GetTickCount())
	{
		dwAttackTime = GetTickCount();
		Vector2 vDirToPlayer = { 1.f , 0.f };
		int bulletCount = 3;
		float angleRange = 30.f;
		float angleStep = angleRange / (bulletCount - 1);
		vDirToPlayer.Normalize();
		Vector2 vBaseDir = vDirToPlayer;

		float startAngle = -angleRange / 2.f;
		for (int i = 0; i < bulletCount; i++)
		{
			float angleDeg = startAngle + i * angleStep;
			float angleRad = angleDeg * (3.14159265f / 180.f);
			float cosA = cosf(angleRad);
			float sinA = sinf(angleRad);

			Vector2 vRotatedDir(vBaseDir.X() * cosA - vBaseDir.Y() * sinA, vBaseDir.X() * sinA + vBaseDir.Y() * cosA);

			CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
				O_ENBULLET, pTransform->Position().X() + vRotatedDir.X() * 5.f, pTransform->Position().Y() + vRotatedDir.Y() * 20.f));

			pBullet->Set_BulletType(CBullet::B04);
			pBullet->Set_EffectType(CBullet::E03);
			pBullet->Apply_BulletSprite();
			pBullet->Apply_EffectAnim();
			pBullet->Set_Direction(vRotatedDir);
			pBullet->Set_Speed(5.f);
		}
	}

}

void CGuide::Guide_SwapWeapon()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E');
		bSwapWeaponCompleted = true;
	}
}

void CGuide::Guide_ItemUse()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_SPACE);
		iCount++;
		if (iCount >= iMaxCount)
			bItemUseCompleted = true;
	}
	if (!bUsedItem)
	{
		bUsedItem = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_LBUTTON);
	}
}

void CGuide::Guide_Interact()
{
	if (!bCanNextDialogue)
	{
		bCanNextDialogue = MANAGER(CInputManager*, M_INPUT)->Get_KeyDown('E');
		iCount++;
		if (iCount >= iMaxCount)
			bInteractCompleted = true;
	}
	if (iBombFallCount++ < 15)
	{
		pBomb->Get_Transform()->Position({ pBomb->Get_Transform()->Position().X(), pBomb->Get_Transform()->Position().Y() + 5.f});
		pCartridge->Get_Transform()->Position({ pCartridge->Get_Transform()->Position().X(), pCartridge->Get_Transform()->Position().Y() + 5.f });
	}
}

void CGuide::Check_NextScript()
{
	// 막 등장 후 첫 대화 시작해야 하는 경우
	if (eGuideStep == GuideStep::Move && eGuideState == GuideState::Left && !bPlayDialogue)
	{
		spriteKey = L"EKey";
		iPanelOffsetX = iPanelOffsetX03;
		iPanelOffsetY = iPanelOffesetY03;
		bPlayDialogue = true;
		iCurLetterIndex = 0;
		iCurScriptIndex = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		dwDialogueElaspedTime = GetTickCount();
	}
	else if (eGuideStep == GuideStep::Move && eGuideState == GuideState::Left && bCanNextDialogue && bMoveCompleted)
	{
		spriteKey = L"MDKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 5;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::Dodge;
	}
	else if (eGuideStep == GuideStep::Dodge && bCanNextDialogue && bDodgeCompleted)
	{
		spriteKey = L"MDKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 6;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::Shot;
	}
	else if (eGuideStep == GuideStep::Shot && bCanNextDialogue && bShotCompleted)
	{
		spriteKey = L"RKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 7;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::Reshot;
	}
	else if (eGuideStep == GuideStep::Reshot && bCanNextDialogue && bReshotCompleted)
	{
		spriteKey = L"QKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 8;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::Interact;
		dwAttackTime = GetTickCount();

		// 아이템 생성
		pBomb = dynamic_cast<CBomb*>(CObjectFactory<CBomb>::Create(O_ITEM));
		pBomb->Drop_Item({ pTransform->Position().X() + 20.f, pTransform->Position().Y() + 30.f });
		pCartridge = dynamic_cast<CCartridge*>(CObjectFactory<CCartridge>::Create(O_ITEM));
		pCartridge->Drop_Item({ pTransform->Position().X() - 50.f, pTransform->Position().Y() + 50.f });
	}
	else if (eGuideStep == GuideStep::Interact && bCanNextDialogue && bInteractCompleted
		&& pBomb->Get_Obtained() && pCartridge->Get_Obtained())
	{
		spriteKey = L"SpaceKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 9;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::ItemUse;
	}
	else if (eGuideStep == GuideStep::ItemUse && bCanNextDialogue && bItemUseCompleted && bUsedItem)
	{
		spriteKey = L"QKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 10;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::Cartridge;
	}
	else if (eGuideStep == GuideStep::Cartridge && bCanNextDialogue && bCartridgeCompleted)
	{
		spriteKey = L"CtrlKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 11;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::SwapWeapon;
	}
	else if (eGuideStep == GuideStep::SwapWeapon && bCanNextDialogue && bSwapWeaponCompleted)
	{
		spriteKey = L"EKey";
		iCurLetterIndex = 0;
		iCurScriptIndex = 12;
		iCount = 0;
		iTotalLength = lstrlen(szScript[iCurScriptIndex]);
		eGuideStep = GuideStep::End;
	}
}

void CGuide::Next_Script()
{
	iCurLetterIndex = 0;
	iTotalLength = lstrlen(szScript[iCurScriptIndex]);

	dwDialogueElaspedTime = GetTickCount();
	fill(begin(szTmp), end(szTmp), 0);
	bPlayDialogue = true;
}
