#pragma once
#include "CItem.h"
class CChest :   public CItem
{
public :
	void Render(HDC hDC) override;

	void Late_Initialize() override;
	void Get_Item() override;
	void Apply_ItemEffect() override;
	void Drop_Item(Vector2 vDropPos) override;

public :
	void OnDetect_PlayerIn() override;
	void OnDetect_PlayerOut() override;

private :
	void Display_Shotgun(HDC hDC);

private :
	int iAnimRow;
	bool bShowShotgun;
	DWORD dwDisplayElapsedTime;
	int iOffsetY;

	bool bDropEffect;
	Vector2 vDroppedPos;
};

