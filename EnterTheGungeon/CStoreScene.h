#pragma once
#include "CScene.h"
class CPlayer;
class CMobOwner;
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
	CMobOwner* pOwner;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;

private:
	RECT rTransitBound;
	bool bDrawTeleport;
	int iAnimCol;
	int iRealSize = 350;
	DWORD dwAnimElapsedTime;
	Vector2 vRealPos = { 1525, 1435 };

private:
	vector<Vector2> vTablePos01 = { {1811, 1078}, { 1811, 1380 }, { 2192, 1078 }, { 2192, 1380 } };

	POINT pPlayerPos = { 1630, 1540 };
	POINT pOwnerPos = { 1120, 1059 };
};

