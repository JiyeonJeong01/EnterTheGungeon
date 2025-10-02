#include "pch.h"
#include "CElavator.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CPlayer.h"
#include "CBmpManager.h"
#include "CObjectFactory.h"
#include "CObjectManager.h"

void CElavator::Initialize()
{
    CObject::Initialize();
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/BulletElevator.bmp", L"BulletElevator");

	eType = O_INTERACTABLE;
	pRenderer->rType = RND__GAMEBOJECT;

	eState = Rising;
	dwElavatorElapsedTime = GetTickCount();

	iAnimCol = 9, iAnimRow = 1;

	iRealSizeX = iRealSizeY = 200;
	iRenderSizeX = iRenderSizeY = 400;

	dwAnimFrameElapsedTime = GetTickCount();
	fFrameRange = 100;
	iMaxAnimCol = 0;

	dynamic_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front())->Set_CanRender(false);
}

int CElavator::Update()
{
	switch (eState)
	{
	case CElavator::Rising:
		Rising_Logic();
		break;
	case CElavator::Opening:
		Opening_Logic();
		break;
	case CElavator::Opened:
		Opened_Logic();
		break;
	}
    return 0;
}

void CElavator::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"BulletElevator");

	GdiTransparentBlt(hDC,
		pRenderer->Right(), pRenderer->Top(),
		iRenderSizeX, iRenderSizeY,
		hMemDC,
		iAnimCol * iRealSizeX, iAnimRow * iRealSizeX,
		iRealSizeX, iRealSizeY,
		RGB(255, 0, 255));
}

void CElavator::Late_Update()
{
	CObject::Update_Renderer();
	Update_AnimFrame();
}

void CElavator::Release()
{
}

void CElavator::Update_AnimFrame()
{
	if (dwAnimFrameElapsedTime + fFrameRange  < GetTickCount())
	{
		dwAnimFrameElapsedTime = GetTickCount();
		if (eState == Rising)
		{
			iAnimCol--;
			if (iAnimCol == 0)
			{
				eState = Opening;
				iAnimRow = 2;
				iMaxAnimCol = 5;
			}
		}
		else if (eState == Opening)
		{
			if ( iAnimCol < iMaxAnimCol)
			{
				iAnimCol++;
			}
			else
			{
				iAnimCol = iMaxAnimCol;
				eState = Opened;
				dynamic_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front())->Set_CanRender(true);

			}
		}


	}
}


void CElavator::Rising_Logic()
{



}

void CElavator::Opening_Logic()
{
}

void CElavator::Opened_Logic()
{
}
