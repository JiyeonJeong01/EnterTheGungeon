#pragma once
#include "CScene.h"
class CPlayer;
class CMapGround;
class CMap;
class CStage01 :public CScene
{
public:
	CStage01();
	~CStage01() override;

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	CPlayer* pPlayer;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;


private :
	RECT rTransitBound;
};

