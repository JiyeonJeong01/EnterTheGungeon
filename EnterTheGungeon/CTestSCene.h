#pragma once
#include "CScene.h"
class CPlayer;
class CMap;

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
	list<CMap*> mapList;
	
};

