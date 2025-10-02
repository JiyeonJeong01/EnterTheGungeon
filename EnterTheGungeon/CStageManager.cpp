#pragma region INCLUDE
#include "pch.h"
#include "CStageManager.h"
#include "CSceneManager.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CMapManager.h"
#include "CInputManager.h"
#include "CBmpManager.h"

#include "CStage01.h"
#include "CStage02.h"
#include "CStoreScene.h"
#include "CPlayer.h"

#include "CMob.h"
#include "CMob01.h"
#include "CMob02.h"
#include "CMob03.h"
#include "CMob04.h"
#include "CMob05.h"
#include "CGuide.h"
#include "CEndingGuide.h"
#include "CFinalChest.h"

#include "CItem.h"
#include "CCoin.h"
#include "CCartridge.h"
#include "CBomb.h"
#include "CChest.h"
#include "CTeleport.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"
#pragma endregion


CStageManager::CStageManager()
{
	pPlayer = nullptr;
}

CStageManager::~CStageManager()
{
}

void CStageManager::Initialize()
{
	bSpawnTimeUp = false;
	bSpawnAimTimeUp = false; 
	bPlaySpawnAnim = false;
}

void CStageManager::Render(HDC hDC)
{
	// 충돌 범위 디버깅
	if (bShouldCheckBound)
	{
		//HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		//HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
		//HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		//Vector2 vLT = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ (float)rCurCheckBound.left, (float)rCurCheckBound.top });
		//Vector2 vRB = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({ (float)rCurCheckBound.right, (float)rCurCheckBound.bottom });
		//Rectangle(hDC, (int)vLT.X(), (int)vLT.Y(), (int)vRB.X(), (int)vRB.Y());
		//SelectObject(hDC, hOldBrush);
		//SelectObject(hDC, hOldPen);
		//DeleteObject(hPen);
	}

	// 포탈 애니메이션 
	if (bPlayAnim && bPlayerInBound)
	{
		if (dwAnimElapsedTime + 70 < GetTickCount())
		{
			iCol++;
			dwAnimElapsedTime = GetTickCount();
			if (iCol > iMaxCol)
			{
				bPlayAnim = false;
			}
		}

		Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos({(float)pPortalAnimPos.x, (float)pPortalAnimPos.y});

		HDC effect = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"TeleportEffect");

		GdiTransparentBlt(hDC, vRenderPos.X() + 20, vRenderPos.Y() -130,  iSizeX, iSizeY, effect, iCol * iSizeX, 0, iSizeX, iSizeY, RGB(32, 32, 32));
	}

	// 스폰 애니메이션
	if (bPlaySpawnAnim)
	{
		if (dwSpawnAimElapsedTime + 30 < GetTickCount())
		{
			iSpawnCol++;
			dwSpawnAimElapsedTime = GetTickCount();

			if (iSpawnCol > iMaxSpawnCol)
			{
				bPlaySpawnAnim = false;
				iSpawnCol = 0;
			}
		}
		Vector2 vRenderPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RenderPos(vSpawnPos);

		HDC spawnEffect = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster_Spawn");
		cout << "현재 열 : " << iSpawnCol << endl;
		GdiTransparentBlt(hDC, vRenderPos.X() , vRenderPos.Y(), iSpawnSizeX * 0.5f, iSpawnSizeY * 0.5f, spawnEffect, iSpawnCol * iSpawnSizeX, 0, iSpawnSizeX, iSpawnSizeY, RGB(255, 0, 255));
		//GdiTransparentBlt(hDC, 0, 0, iSizeX, iSizeY, spawnEffect, 0, 0, iSpawnSizeX, iSpawnSizeY, RGB(255, 0, 255));
	}

}

void CStageManager::Release()
{
}

void CStageManager::Initialize_Stage01()
{
	bCanTransitNextStage = false;
	bPreparedTransit = false;
	bShouldSpawn = false;
	bShouldCheckBound = true;
	rCurCheckBound = rSpawnTrigger01_01;

	bSpawnTimerOn = false;
	bSpawnAimTimerOn = false;
	fSpawnRange = 1.2f;
	fSpawnAnimRange = 0.2;

	iCurrentKillCount = 0;

	iSpawnCol = 0;
	bPlaySpawnAnim = false;

	bPlayAnim = false;

	if (pPlayer == nullptr)
	{
		pPlayer = static_cast<CPlayer*>(
			MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
	}

	eCurStage = MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene();
	eCurStageState = Entered;
	Change_State();

	iSpawnCol = 0;
	bClearStage = false;
}

void CStageManager::Initialize_Stage02()
{
	bCanTransitNextStage = false;
	bPreparedTransit = false;
	bShouldSpawn = false;
	bShouldCheckBound = true;
	bSpawnTimerOn = false;
	bSpawnAimTimerOn = false;
	fSpawnRange = 0.8f;

	bPlayAnim = false;

	iSpawnCol = 0;
	bPlaySpawnAnim = false;

	iCurrentKillCount = 0;

	if (pPlayer == nullptr)
	{
		pPlayer = static_cast<CPlayer*>( MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
	}

	eCurStage = MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene();
	eCurStageState = Entered;
	Change_State();
}

void CStageManager::Initialize_Store()
{
	bCanTransitNextStage = true;
	bPreparedTransit = false;
	bShouldSpawn = false;
	bShouldCheckBound = true;
	bSpawnTimerOn = false;

	bPlayAnim = false;

	if (pPlayer == nullptr)
	{
		pPlayer = static_cast<CPlayer*>(MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front());
	}

	eCurStage = MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene();
	eCurStageState = Entered;
	Change_State();

	rCurCheckBound = rTransitBound03;
}

void CStageManager::Initialize_BossStage()
{
	bClearStage = false;
	bReward = true;
}

void CStageManager::Render_SpawnEffect(HDC hDC, int iX, int iY)
{
}

void CStageManager::Logic_Stage01()
{

	if (bSpawnTimerOn) Tick_Spawn();

	if (bShouldCheckBound) Check_PlayerInRect();

	if (bPlayerInBound && eCurStageState == Entered)
	{
		Spawn01_01();
	}

	if (bPlayerInBound && eCurStageState == Spawned01)
	{
		bShouldSpawn = true;
	}
	else if (bShouldSpawn && eCurStageState == Spawned01)
	{
		Spawn01_02();
	}
	else if (bShouldSpawn && eCurStageState == Spawned02)
	{
		Spawn01_03();
	}
	else if (bShouldSpawn && eCurStageState == Spawned03)
	{
		Spawn01_04();
	}
	else if (bPreparedTransit == false && eCurStage == SC_STAGE01 && bCanTransitNextStage)
	{
		Prepare_Store();
	}
	else if (bPreparedTransit && bCanTransitNextStage && bPlayerInBound)
	{
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RETURN))
		{
			Transit_Store();
		}
	}
}

void CStageManager::Logic_Stage02()
{
	if (bSpawnTimerOn) Tick_Spawn();

	if (bShouldCheckBound) Check_PlayerInRect();

	if (bPlayerInBound)
	{
		bShouldSpawn = true;
	}
	if (bShouldSpawn && eCurStageState == Entered)
	{
		Spawn02_01();
	}
	else if (bShouldSpawn && eCurStageState == Spawned01)
	{
		Spawn02_02();
	}
	else if (bShouldSpawn && eCurStageState == Spawned02)
	{
		Spawn02_03();
	}
	else if (bShouldSpawn && eCurStageState == Spawned03)
	{
		Spawn02_04();
	}
	else if (bPreparedTransit == false && eCurStage == SC_STAGE02 && bCanTransitNextStage)
	{
		Prepare_BossStage();
	}
	else if (bPreparedTransit && bCanTransitNextStage && bPlayerInBound)
	{
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RETURN))
		{
			Transit_BossStage();
		}
	}
}

void CStageManager::Logic_Store()
{
	if (bShouldCheckBound) Check_PlayerInRect();

	if (bPreparedTransit == false && eCurStage == SC_STORE && bCanTransitNextStage && CItem::Get_IemFree())
	{
		Prepare_Stage02();
	}
	else if (bPreparedTransit && bCanTransitNextStage && bPlayerInBound)
	{
		if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RETURN))
		{
			Transit_Stage02();
		}
	}

}

void CStageManager::Logic_BossStage()
{
	if (bShouldCheckBound)
	{
		Check_PlayerInRect();
	}
	if (bClearStage && bPlayerInBound && !bReward)
	{
		Vector2 vPos = { 4640, 620 };
		CFinalChest* pReward = static_cast<CFinalChest*>(CObjectFactory<CFinalChest>::Create(O_ITEM));
		pReward->Drop_Item({ vPos.X(), vPos.Y() });
		bReward = true;
	}

}

void CStageManager::On_BossDead()
{
	CObjectFactory<CEndingGuide>::Create(O_ENEMY, 4600, 2700);
	bClearStage = true;
	rCurCheckBound = { 4700, 820, 4900, 920 };
	bShouldCheckBound = true;
}

void CStageManager::Prepare_Stage02()
{
	bPreparedTransit = true;

	static_cast<CStage01*>(MANAGER(CSceneManager*, M_SCENE)->Get_Scene())->Set_TeleportOn();
	bPlayAnim = true;
	pPortalAnimPos = { 1525, 1435 };
	iCol = 0;
	dwAnimElapsedTime = GetTickCount();
}

void CStageManager::Prepare_Store()
{
	Vector2 pPos = pPlayer->Get_Transform()->Position();
	CItem* pItem = static_cast<CItem*>(CObjectFactory<CChest>::Create(O_ITEM));
	pItem->Drop_Item({ pPos.X(), pPos.Y() + 30.f });

	bPreparedTransit = true;
	static_cast<CStage01*>(MANAGER(CSceneManager*, M_SCENE)->Get_Scene())->Set_TeleportOn();

	bPlayAnim = true;
	pPortalAnimPos = { 4780, 3640 };
	iCol = 0;
	dwAnimElapsedTime = GetTickCount();
}

void CStageManager::Prepare_BossStage()
{
	bPreparedTransit = true;
	static_cast<CStage02*>(MANAGER(CSceneManager*, M_SCENE)->Get_Scene())->Set_TeleportOn();

	bPlayAnim = true;
	pPortalAnimPos = { 6010, 2300 };
	iCol = 0;
	dwAnimElapsedTime = GetTickCount();
}

void CStageManager::Transit_Stage02()
{

	MANAGER(CEnvironmentManager*, M_MAP)->Release();
	MANAGER(CSceneManager*, M_SCENE)->Change_Scene(SC_STAGE02);
	MANAGER(CEnvironmentManager*, M_MAP)->Load_Data();

}

void CStageManager::Transit_Store()
{
	pGuideNPC->Set_Dead();

	MANAGER(CEnvironmentManager*, M_MAP)->Release();
	MANAGER(CSceneManager*, M_SCENE)->Change_Scene(SC_STORE);
	MANAGER(CEnvironmentManager*, M_MAP)->Load_Data();
}

void CStageManager::Transit_BossStage()
{
	MANAGER(CEnvironmentManager*, M_MAP)->Release();
	MANAGER(CSceneManager*, M_SCENE)->Change_Scene(SC_TEST);
	MANAGER(CEnvironmentManager*, M_MAP)->Load_Data();
}

void CStageManager::Tick_Spawn()
{
	if (dwSpawnElapsedTime + fSpawnRange * 1000 < GetTickCount())
	{
		dwSpawnElapsedTime = GetTickCount();
		bSpawnTimeUp = true;
	}
}

void CStageManager::Change_State()
{
	if (eCurStage == SC_STAGE01)
	{
		switch (eCurStageState)
		{
		case CStageManager::Entered:
			break;
		case CStageManager::Spawned01: // npc
			bShouldCheckBound = true;
			bPlayerInBound = false;
			rCurCheckBound = rSpawnTrigger01_02;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		case CStageManager::Spawned02:
			bShouldCheckBound = false;
			bShouldSpawn = false;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		case CStageManager::Spawned03:
			bShouldCheckBound = false;
			bShouldSpawn = false;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		}
	}
	else if (eCurStage == SC_STAGE02)
	{
		switch (eCurStageState)
		{
		case CStageManager::Entered:
			bShouldCheckBound = true;
			bShouldSpawn = false;
			rCurCheckBound = rSpawnTrigger02_01;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		case CStageManager::Spawned01:
			bShouldCheckBound = true;
			bShouldSpawn = false;
			rCurCheckBound = rSpawnTrigger02_02;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		case CStageManager::Spawned02:
			bShouldCheckBound = true;
			bShouldSpawn = false;
			rCurCheckBound = rSpawnTrigger02_03;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		case CStageManager::Spawned03:
			bShouldCheckBound = true;
			rCurCheckBound = rSpawnTrigger02_04;
			bShouldSpawn = false;
			bSpawnTimerOn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurSpawnCount = 0;
			break;
		}
	}
}

void CStageManager::Check_PlayerInRect()
{
	Vector2 vPos = pPlayer->Get_Transform()->Position();

	bPlayerInBound = (vPos.X() >= rCurCheckBound.left && vPos.X() <= rCurCheckBound.right 
									&& vPos.Y() >= rCurCheckBound.top && vPos.Y() <= rCurCheckBound.bottom);
}

void CStageManager::OnKilled_Enemy(CMob* pMob)
{
	iCurrentKillCount++;

	Vector2 vItemPos = pMob->Get_Transform()->Position();

	if (eCurStage != SC_STORE)
	{
		// 코인 두 개 고정
		CItem* pItem1 = static_cast<CItem*>(CObjectFactory<CCoin>::Create(O_ITEM, vItemPos.X(), vItemPos.Y()));
		pItem1->Drop_Item({ vItemPos.X() - 20.f, vItemPos.Y() - 10.f });
		CItem* pItem2 = static_cast<CItem*>(CObjectFactory<CCoin>::Create(O_ITEM, vItemPos.X(), vItemPos.Y()));
		pItem2->Drop_Item({ vItemPos.X() + 10.f, vItemPos.Y() + 10.f });
	}

	if (eCurStage == SC_STAGE01)
	{
		if (eCurStageState == Spawned02 && iCurrentKillCount == 4)
		{
			bShouldSpawn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
		else if (eCurStageState == Spawned03 && iCurrentKillCount == 5)
		{
			bShouldSpawn = true;
			dwSpawnElapsedTime = GetTickCount();
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
		else if (eCurStageState == Spawned04 && iCurrentKillCount == 4)
		{
			bShouldSpawn = true;
			bShouldCheckBound = true;
			bCanTransitNextStage = true;
			bPlayerInBound = false;
			rCurCheckBound = rTransitBound01;
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
	}
	else if (eCurStage == SC_STAGE02)
	{
		if (eCurStageState == Spawned01 && iCurrentKillCount == 2)
		{
			bShouldSpawn = false;
			dwSpawnElapsedTime = GetTickCount();
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
		else if (eCurStageState == Spawned02 && iCurrentKillCount == 3)
		{
			bShouldSpawn = false;
			dwSpawnElapsedTime = GetTickCount();
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
		else if (eCurStageState == Spawned03 && iCurrentKillCount == 5)
		{
			bShouldSpawn = false;
			dwSpawnElapsedTime = GetTickCount();
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
		else if (eCurStageState == Spawned04 && iCurrentKillCount == 3)
		{
			bShouldSpawn = false;
			bShouldCheckBound = true;
			bCanTransitNextStage = true;
			bPlayerInBound = false;
			rCurCheckBound = rTransitBound02;
			iCurrentKillCount = 0;
			iCurSpawnCount = 0;
		}
	}
}

void CStageManager::Reward(Vector2 vPos)
{
}

void CStageManager::Spawn01_01()
{
	POINT pSpawnPos = { 5050, 1300 };
	pGuideNPC = static_cast<CMob*>(CObjectFactory<CGuide>::Create(O_ENEMY, pSpawnPos.x, pSpawnPos.y));

	eCurStageState = Spawned01;
	Change_State();
	dwSpawnAimElapsedTime = GetTickCount();
	bPlaySpawnAnim = false;
}

void CStageManager::Spawn01_02()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 3)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos01_02[0].X(), vSpawnPos01_02[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_02[1];
			break;

		case 1:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos01_02[1].X(), vSpawnPos01_02[1].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_02[2];

			break;
		case 2:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos01_02[2].X(), vSpawnPos01_02[2].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_02[3];
			break;
		case 3:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos01_02[3].X(), vSpawnPos01_02[3].Y());
			eCurStageState = Spawned02;
			Change_State();
			break;
		}
		iCurSpawnCount++;

	}
}

void CStageManager::Spawn01_03()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 4)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob04>::Create(O_ENEMY, vSpawnPos01_03[0].X(), vSpawnPos01_03[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_03[1];
			break;
		case 1:
			CObjectFactory<CMob05>::Create(O_ENEMY, vSpawnPos01_03[1].X(), vSpawnPos01_03[1].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_03[2];
			break;
		case 2:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos01_03[2].X(), vSpawnPos01_03[2].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_03[3];
			break;
		case 3:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos01_03[3].X(), vSpawnPos01_03[3].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_03[4];
			break;
		case 4:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos01_03[4].X(), vSpawnPos01_03[4].Y());
			eCurStageState = Spawned03;
			Change_State();
			break;
		}
		iCurSpawnCount++;
	}
}

void CStageManager::Spawn01_04()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 4)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob04>::Create(O_ENEMY, vSpawnPos01_04[0].X(), vSpawnPos01_04[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_04[1];
			break;
		case 1:
			CObjectFactory<CMob05>::Create(O_ENEMY, vSpawnPos01_04[1].X(), vSpawnPos01_04[1].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_04[2];
			break;
		case 2:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos01_04[2].X(), vSpawnPos01_04[2].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos01_04[3];
			break;
		case 3:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos01_04[3].X(), vSpawnPos01_04[3].Y());

			eCurStageState = Spawned04;
			Change_State();
			break;
		}
		iCurSpawnCount++;
	}
}

void CStageManager::Spawn02_01()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 1)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos02_01[0].X(), vSpawnPos02_01[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_01[1];
			break;
		case 1:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos02_01[1].X(), vSpawnPos02_01[1].Y());
			eCurStageState = Spawned01;
			Change_State();
			break;
		}
		iCurSpawnCount++;
	}
}

void CStageManager::Spawn02_02()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 2)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos02_02[0].X(), vSpawnPos02_02[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_02[1];
			break;
		case 1:
			CObjectFactory<CMob05>::Create(O_ENEMY, vSpawnPos02_02[1].X(), vSpawnPos02_02[1].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_02[2];
			break;
		case 2:
			CObjectFactory<CMob04>::Create(O_ENEMY, vSpawnPos02_02[2].X(), vSpawnPos02_02[2].Y());
			eCurStageState = Spawned02;
			Change_State();
			break;
		}
		iCurSpawnCount++;

	}
}

void CStageManager::Spawn02_03()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 4)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos02_03[0].X(), vSpawnPos02_03[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_03[1];
			break;
		case 1:
			CObjectFactory<CMob01>::Create(O_ENEMY, vSpawnPos02_03[1].X(), vSpawnPos02_03[1].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_03[2];
			break;
		case 2:
			CObjectFactory<CMob02>::Create(O_ENEMY, vSpawnPos02_03[2].X(), vSpawnPos02_03[2].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_03[3];
			break;
		case 3:
			CObjectFactory<CMob04>::Create(O_ENEMY, vSpawnPos02_03[3].X(), vSpawnPos02_03[3].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_03[4];
			break;
		case 4:
			CObjectFactory<CMob05>::Create(O_ENEMY, vSpawnPos02_03[4].X(), vSpawnPos02_03[4].Y());

			eCurStageState = Spawned03;
			Change_State();
			break;
		}
		iCurSpawnCount++;
	}
}

void CStageManager::Spawn02_04()
{
	if (bSpawnTimeUp && iCurSpawnCount <= 2)
	{
		bSpawnTimeUp = false;
		dwSpawnElapsedTime = GetTickCount();
		switch (iCurSpawnCount)
		{
		case 0:
			CObjectFactory<CMob03>::Create(O_ENEMY, vSpawnPos02_04[0].X(), vSpawnPos02_04[0].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_04[1];
			break;
		case 1:
			CObjectFactory<CMob03>::Create(O_ENEMY, vSpawnPos02_04[1].X(), vSpawnPos02_04[1].Y());
			bPlaySpawnAnim = true;
			vSpawnPos = vSpawnPos02_04[2];
			break;
		case 2:
			CObjectFactory<CMob03>::Create(O_ENEMY, vSpawnPos02_04[2].X(), vSpawnPos02_04[2].Y());
			eCurStageState = Spawned04;
			Change_State();
			break;
		}
		iCurSpawnCount++;
	}
}
