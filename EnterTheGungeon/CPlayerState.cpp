#include "pch.h"
#include "CPlayerState.h"
#include "CRenderer.h"
#include "CTransform.h"

void CPlayerState::Update()
{
	// load player's info
	vInputDir = static_cast<CPlayer*>(pObj)->vInputDir;
	pInputCursor = static_cast<CPlayer*>(pObj)->pInputCursor;
	dwLastFireTime = static_cast<CPlayer*>(pObj)->dwLastFireTime;
	fSpeed = static_cast<CPlayer*>(pObj)->fSpeed;
	bDodgePlaying = static_cast<CPlayer*>(pObj)->bDodgePlaying;
	eDir = static_cast<CPlayer*>(pObj)->eDir;
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
