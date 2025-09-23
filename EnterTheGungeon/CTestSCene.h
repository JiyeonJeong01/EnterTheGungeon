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

public :
	CPlayer* pPlayer;
	CBoss* pBoss;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;
	
};

