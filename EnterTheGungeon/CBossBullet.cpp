#include "pch.h"
#include "CBossBullet.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

void CBossBullet::Initialize()
{
	CBullet::Initialize();

	eType = O_ENBULLET;
	pRenderer->rType = RND__GAMEBOJECT;

	pTransform->Size({ 40.f, 40.f });
	pCollider->Size({ 30.f, 30.f });
	pRenderer->Size({ 40.f, 40.f });

	rSummoned = false;
}

int CBossBullet::Update()
{
	CBullet::Update();

	if (eBulletType == CBullet::BossBullet)
	{
		Vector2 curPos = pTransform->Position();
		if (curPos.X() >= rSummonBound.left &&
			curPos.X() <= rSummonBound.right &&
			curPos.Y() >= rSummonBound.top &&
			curPos.Y() <= rSummonBound.bottom)
		{
			pTransform->Direction({ 0.f, 0.f });
			bCanRend = false;
		}

	}


	return 0;
}
