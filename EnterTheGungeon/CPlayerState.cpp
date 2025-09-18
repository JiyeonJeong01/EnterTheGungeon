#include "pch.h"
#include "CPlayerState.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CObjectFactory.h"
#include "CPlayerBullet.h"
#include "CInputManager.h"

void CPlayerState::Update()
{
	// load player's info
	vInputDir = static_cast<CPlayer*>(pObj)->vInputDir;
	pInputCursor = static_cast<CPlayer*>(pObj)->pInputCursor;
	dwLastFireTime = static_cast<CPlayer*>(pObj)->dwLastFireTime;
	fLimitFireTime = static_cast<CPlayer*>(pObj)->fLimitFireTime;
	fSpeed = static_cast<CPlayer*>(pObj)->fSpeed;
	bDodgePlaying = static_cast<CPlayer*>(pObj)->bDodgePlaying;
	eDir = static_cast<CPlayer*>(pObj)->eDir;

	if (dwLastFireTime + fLimitFireTime * 1000 < GetTickCount())
		bCanShot = true;

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_LBUTTON))
	{
		Shot_Bullet();
	}
}

void CPlayerState::Render_Player(HDC hDC)
{
	CRenderer renderer = *(pObj->Get_Renderer());
	CTransform transform = *(pObj->Get_Transform());

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)transform.Size().X(),
		(int)transform.Size().Y(),
		hDC,
		animation.iCurrIndex * (int)transform.Size().X(),
		animation.iDirRow * (int)transform.Size().Y(),
		(int)transform.Size().X(),
		(int)transform.Size().Y(),
		RGB(255, 0, 255));
}

void CPlayerState::Shot_Bullet()
{
	if (!bCanShot && bDodgePlaying)
		return;

	CPlayerBullet* pBullet = dynamic_cast<CPlayerBullet*>(CObjectFactory<CPlayerBullet>::Create(
		O_PLBULLET, pObj->Get_Transform()->Position().X(), pObj->Get_Transform()->Position().Y()));

	pBullet->Set_BulletType(CBullet::B01);
	pBullet->Set_EffectType(CBullet::E01);
	pBullet->Apply_BulletSprite();
	pBullet->Apply_EffectAnim();

	// for test sibar
	POINT curPos = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 dir = {pObj->Get_Transform()->Position().X() - (float)curPos.x,  pObj->Get_Transform()->Position().Y() - (float)curPos.y };
	dir.Normalize();
	pBullet->Set_Direction(dir * -1.f);
	pBullet->Set_Speed(8.f);

}
