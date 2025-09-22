#pragma once
#include "CItem.h"
class CCartridge :  public CItem
{
	void Late_Initialize() override;
	void Get_Item() override;
	void Apply_ItemEffect() override;
};
