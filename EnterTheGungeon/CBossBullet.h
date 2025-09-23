#pragma once
#include "CBullet.h"
class CBossBullet :   public CBullet
{
public :
	void Initialize() override;
	int Update() override;

public :
	RECT rSummonBound;
	bool rSummoned;
};

