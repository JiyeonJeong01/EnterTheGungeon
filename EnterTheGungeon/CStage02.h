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
	CPlayer* pPlayer;
	list<CMap*> mapObjList;
	list<CMapGround*> mapList;
};

