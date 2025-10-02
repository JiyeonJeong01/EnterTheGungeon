#pragma once
#include "CScene.h"
class CPlayer;
class CMapGround;
class CMap;
class CStage02 : public CScene
{
public:
	CStage02();
	~CStage02() override;

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	void Set_TeleportOn();

private :
	void Detect_Collision();

private:
	void Place_Objects();
	void Draw_Teleport(HDC hDC);


public:
	CPlayer* pPlayer;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;

private :
	bool bDrawTeleport;
	int iAnimCol;
	int iRealSize = 350;
	DWORD dwAnimElapsedTime;
	Vector2 vRealPos = { 6010, 2300 };

private:
	vector<Vector2> vTablePos01 = { {810, 670}, { 470, 670 },
														{ 1728, 1704 }, { 2316, 1704 }, { 2877, 1704 },  { 1728, 2294 }, { 2316, 2294 }, { 2877, 2294 },
														{ 5347, 2098 }, {5347, 2508} };
};

