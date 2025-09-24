#pragma once
#include "CBullet.h"
class CBossBullet :   public CBullet
{
public :
	~CBossBullet() override;
public :
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	void OnCollision_MapGround() override;

public : 
	void Set_SummonBulletIndex(int index) { iSummonBulletIndex = index; }
public :
	RECT rSummonBound;
	bool rSummoned;
	int iSummonBulletIndex;
	bool bSummonCanDestoryed;
};

