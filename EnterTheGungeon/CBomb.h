#pragma once
#include "CItem.h"
class CBomb :   public CItem
{
	void Late_Initialize() override;
	void Get_Item() override;
	void Apply_ItemEffect() override;
};

