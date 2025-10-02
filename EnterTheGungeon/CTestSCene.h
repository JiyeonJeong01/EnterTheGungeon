#pragma once
#include "CScene.h"
class CPlayer;
class CMapGround;
class CMap;
class CBoss;
class CTestSCene : public CScene
{
public:
	CTestSCene();
	~CTestSCene() override;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

private :
	void Place_Objects();
	void Draw_Teleport(HDC hDC);

public :
	CPlayer* pPlayer;
	CBoss* pBoss;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;

private :
	POINT pPlayerPos = { 3000, 3050 };
	POINT pBossPos = { pPlayerPos.x + 1500, pPlayerPos.y - 200 };

	vector<Vector2> vTablePos01 = { {4215, 2428}, { 4521, 2428 }, { 5079, 2428 }, { 5421, 2428 },
														 {4215, 3112}, { 4521, 3112 }, { 5079, 3112 }, { 5421, 3112 } };

	bool bDrawTeleport;
	int iAnimCol;
	int iRealSize = 350;
	DWORD dwAnimElapsedTime;
	Vector2 vRealPos = { 3000, 3050 };
};

