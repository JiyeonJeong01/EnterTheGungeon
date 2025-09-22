#pragma once
#include "CItem.h"
class CCoin :  public CItem
{
public :
	int Update() override;
	void OnDetect_Player() override;
	void Late_Initialize() override;
	void Get_Item() override;
	void Apply_ItemEffect() override;

public :
	void Chase_Player();

private :
	bool bChase;
	float fSpeed;
};

