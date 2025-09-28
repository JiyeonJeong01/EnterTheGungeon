#pragma once
#include "CItem.h"
class CCartridge :  public CItem
{
public :
	void Late_Initialize() override;
	void Get_Item() override;
	void Apply_ItemEffect() override;

public :
	void Display_ItemInfo(HDC hDC) override;
	void OnDetect_PlayerIn() override;
	void OnDetect_PlayerOut() override;

};
