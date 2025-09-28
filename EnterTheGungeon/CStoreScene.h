#pragma once
#include "CScene.h"
class CPlayer;
class CMapGround;
class CMap;
class CStoreScene :public CScene
{
public:
	CStoreScene();
	~CStoreScene() override;

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	void Set_TeleportOn();

private:
	void Detect_Collision();

private:
	void Place_Objects();
	void Draw_Teleport(HDC hDC);

public:
	CPlayer* pPlayer;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;

private:
	RECT rTransitBound;
	bool bDrawTeleport;
	int iAnimCol;
	int iRealSize = 350;
	DWORD dwAnimElapsedTime;
	Vector2 vRealPos = { 4780, 3640 };

private:
	vector<Vector2> vTablePos01 = { {4460, 1100}, { 4460, 1380 }, { 5420, 1100 }, { 5420, 1380 } };
};

