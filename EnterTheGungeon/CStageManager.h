#pragma once
#include "CManager.h"

class CPlayer;
class CMob; 

class CStageManager :  public CManager
{
public :
	enum StageState { Entered, Spawned01, Spawned02, Spawned03, Spawned04, Cleared };
public :
	CStageManager();
	~CStageManager() override;

public :
	void Initialize() override;
	void Render(HDC hDC);
	void Release() override;

public:
	void Initialize_Stage01();
	void Initialize_Stage02();
	void Render_SpawnEffect(HDC hDC, int iX, int iY);

public :
	void Logic_Stage01();
	void Logic_Stage02();

private :
	void Prepare_Stage02();
	void Prepare_BossStage();
	void Transit_Stage02();
	void Transit_BossStage();

public :
	void OnKilled_Enemy(CMob* pMob);


private :
	void Tick_Spawn();

private :
	void Change_State();
	void Check_PlayerInRect();
	
	void Reward(Vector2 vPos);

private :
	void Spawn01_01();
	void Spawn01_02();
	void Spawn01_03();
	void Spawn01_04();
	void Spawn02_01();
	void Spawn02_02();
	void Spawn02_03();
	void Spawn02_04();

private :
	CPlayer* pPlayer;

private :
	int iCurrentKillCount;
	bool bCanTransitNextStage;
	bool bPreparedTransit;

private :
	DWORD dwSpawnElapsedTime;
	DWORD dwSpawnAimElapsedTime;
	float fSpawnRange;
	float fSpawnAnimRange;

	bool bSpawnTimerOn;
	bool bSpawnAimTimerOn;

	bool bSpawnTimeUp;
	bool bSpawnAimTimeUp;

	int iCurSpawnCount;
	bool bShouldSpawn;

private :
	SceneType eCurStage;
	StageState eCurStageState;

	RECT rCurCheckBound;
	bool bShouldCheckBound;
	bool bPlayerInBound;

	RECT rTransitBound01 ={ 4700, 3700, 5100, 3800 };
	RECT rTransitBound02 = { 4700, 3700, 5100, 3800 };

	//							stage _ num
	RECT rSpawnTrigger01_01; // for npc
	RECT rSpawnTrigger01_02 = { 4700, 2500, 5000, 2780 };	 // 첫번째 스폰 - mob01 4마리 + mob02 1마리

	RECT rSpawnTrigger02_01 = { 520, 528, 780, 700 }; 
	RECT rSpawnTrigger02_02 = { 1410, 2040, 1610, 2300 }; 
	RECT rSpawnTrigger02_03 = { 2700, 1740, 3100, 2100 }; 
	RECT rSpawnTrigger02_04 = { 5100, 2230, 5400, 2340 };

	Vector2 vSpawnPos01_01;
	Vector2 vSpawnPos01_02[4] = { {4350, 2680}, { 4620, 2930 }, { 5125, 2930 }, { 5450, 2610 } };
	Vector2 vSpawnPos01_03[5] = { { 4260, 3129 }, { 4271, 3287 }, { 4579, 3513 }, { 4470, 2894 }, {4300, 3055} };
	Vector2 vSpawnPos01_04[4] = { { 5260, 3129 }, { 5271, 3287 }, { 5579, 3513 }, {5300, 3055} };

	Vector2 vSpawnPos02_01[2] = { {385, 837}, { 820, 837 } };
	Vector2 vSpawnPos02_02[3] = { {1848, 1775}, { 2150, 2010 }, { 1848, 2300 } };
	Vector2 vSpawnPos02_03[5] = { { 3500, 1810 }, { 3500, 2222 }, { 2500, 2222 }, { 2145, 1800 }, {2430, 1500} };
	Vector2 vSpawnPos02_04[3] = { { 5550, 2300 }, { 6080, 2300 }, { 6500, 2280 }};

};

