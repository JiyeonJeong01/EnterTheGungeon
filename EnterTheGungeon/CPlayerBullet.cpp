#include "pch.h"
#include "CPlayerBullet.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"

void CPlayerBullet::Initialize()
{
	CBullet::Initialize();

	eType = O_PLBULLET;
	pRenderer->rType = RND__GAMEBOJECT;

	pTransform->Size({ 30.f, 30.f });
	pCollider->Size({ 20.f, 20.f });
	pRenderer->Size({ 30.f, 30.f });

}
