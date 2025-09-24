#include "pch.h"
#include "CBossBullet.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

#include "CBmpManager.h"

CBossBullet::~CBossBullet()
{
	CBullet::Release();
}

void CBossBullet::Initialize()
{
	CBullet::Initialize();

	eType = O_ENBULLET;
	pRenderer->rType = RND__GAMEBOJECT;

	pTransform->Size({ 40.f, 40.f });
	pCollider->Size({ 30.f, 30.f });
	pRenderer->Size({ 40.f, 40.f });

	rSummoned = false;
	bSummonCanDestoryed = false;
}

int CBossBullet::Update()
{
	if (CBullet::Update() == S_DEAD) return S_DEAD ;

	if (eBulletType == CBullet::Boss_Summon)
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

void CBossBullet::Render(HDC hDC)
{
	if (eBulletType == Boss_Summon)
	{
		CObject::Render(hDC);

		if (!bCanRend) return;

		HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(spriteKey);
		this;
		GdiTransparentBlt(hDC,
			pRenderer->Left(),
			pRenderer->Top(),
			(int)pRenderer->Size().X(),
			(int)pRenderer->Size().Y(),
			hMemDC,
			iSummonBulletIndex * pRenderer->Size().X(), 0,
			(int)pRenderer->Size().X(),
			(int)pRenderer->Size().Y(),
			RGB(255, 0, 255));
	}
	else
	{
		CBullet::Render(hDC);
	}
}

void CBossBullet::OnCollision_MapGround()
{
	if (eBulletType != Boss_Summon || bSummonCanDestoryed)
	{
		CBullet::OnCollision_MapGround();
	}
}
