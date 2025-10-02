#pragma once
#include "CItem.h"
class CObject;
class CBoomerang : public CItem
{
public:
	void Late_Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	void Get_Item() override;
	void Apply_ItemEffect() override;

public:
	void Display_ItemInfo(HDC hDC) override;
	void OnDetect_PlayerIn() override;
	void OnDetect_PlayerOut() override;

private :
	void Attack_Enemies();
	void Render_Path(HDC hDC);

private :
	bool bBoomerangWorking;
	bool bBoomerangReturn;
	Vector2 vStartPos;
	list<CObject*> pDetectedEnemies;
	list<CObject*>::iterator itCurEnemy;

	DWORD dwAnimElapsedTime;
};

