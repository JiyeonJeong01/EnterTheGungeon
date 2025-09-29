#pragma region INCLUDE
#include "pch.h"
#include "CPlayerState.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CObjectFactory.h"
#include "CPlayerBullet.h"
#include "CInputManager.h"
#include "CCameraManager.h"
#include "CPlayerWeapon.h"
#include "CMouse.h"
#include "CMob.h"
#pragma endregion


void CPlayerState::Update()
{
	Get_WeaponDir();
	// load player's info
	vInputDir = static_cast<CPlayer*>(pObj)->vInputDir;
	pInputCursor = static_cast<CPlayer*>(pObj)->pInputCursor;
	dwLastFireTime = static_cast<CPlayer*>(pObj)->dwLastFireTime;
	fLimitFireTime = static_cast<CPlayer*>(pObj)->fLimitFireTime;
	fSpeed = static_cast<CPlayer*>(pObj)->fSpeed;
	bDodgePlaying = static_cast<CPlayer*>(pObj)->bDodgePlaying;
	bReloading = static_cast<CPlayer*>(pObj)->bReloading;

	bCanShotGun = static_cast<CPlayer*>(pObj)->bCanShotGun;
	bCanShotBomb = static_cast<CPlayer*>(pObj)->bCanShotBomb;

	Dir_ByCursor();

	if (dwLastFireTime + fLimitFireTime * 1000 < GetTickCount())
		bCanShotGun = true;

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_LBUTTON))
	{
		Shot_Bullet();
	}
}

void CPlayerState::Render_Player(HDC hDC)
{

}

void CPlayerState::Get_WeaponDir()
{
	POINT pCursor = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 vCursorPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)pCursor.x, (float)pCursor.y });
	Vector2 vPlayerPos = pObj->Get_Transform()->Position();

	Vector2 vDiff = vCursorPos - vPlayerPos;
	float fDist = sqrtf(vDiff.X() * vDiff.X() + vDiff.Y() * vDiff.Y());
	float theta = acosf(vDiff.X() / fDist);
	float degree = theta * (180.0f / 3.14159265f);

	iWeaponRowIndex = (vDiff.X() > 0) ? 0 : 1;

	if (vDiff.Y() < 0) // 위쪽
	{
		if ((degree >= 0.f && degree <= 15.f) || (degree >= 165.f && degree <= 180.f))
			iWeaponColIndex = 5;
		else if ((degree > 15.f && degree <= 30.f) || (degree >= 150.f && degree < 165.f))
			iWeaponColIndex = 4;
		else if ((degree > 30.f && degree <= 45.f) || (degree >= 135.f && degree < 150.f))
			iWeaponColIndex = 3;
		else if ((degree > 45.f && degree <= 60.f) || (degree >= 120.f && degree < 135.f))
			iWeaponColIndex = 2;
		else if ((degree > 60.f && degree <= 75.f) || (degree >= 105.f && degree < 120.f))
			iWeaponColIndex = 1;
		else // (75~90) or (90~105)
			iWeaponColIndex = 0;
	}
	else // 아래쪽
	{
		if ((degree >= 0.f && degree <= 15.f) || (degree >= 165.f && degree <= 180.f))
			iWeaponColIndex = 5;
		else if ((degree > 15.f && degree <= 30.f) || (degree >= 150.f && degree < 165.f))
			iWeaponColIndex = 6;
		else if ((degree > 30.f && degree <= 45.f) || (degree >= 135.f && degree < 150.f))
			iWeaponColIndex = 7;
		else if ((degree > 45.f && degree <= 60.f) || (degree >= 120.f && degree < 135.f))
			iWeaponColIndex = 8;
		else if ((degree > 60.f && degree <= 75.f) || (degree >= 105.f && degree < 120.f))
			iWeaponColIndex = 9;
		else // (75~90) or (90~105)
			iWeaponColIndex = 9;
	}
}

void CPlayerState::Attack_Bomb()
{
	POINT p = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 vRealPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({(float)p.x, (float)p.y});
	static_cast<CPlayer*>(pObj)->Set_ShotMode(CPlayer::Gun);
	MANAGER(CUIManager*, M_UI)->Get_Mouse()->Set_CursorMode(CMouse::Normal);
	MANAGER(CCameraManager*, M_CAMERA)->Set_CamerMode(CCameraManager::Shake2);

	int iExplosionSize = 300;
	RECT rBombBound = { vRealPos.X() - iExplosionSize, vRealPos.Y() - iExplosionSize, vRealPos.X() + iExplosionSize, vRealPos.Y() + iExplosionSize };

	list<CObject*> pEnemies = *MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENEMY);
	for (auto& p : pEnemies)
	{
		Vector2 v = p->Get_Transform()->Position();
		if (v.X() >= rBombBound.left && v.X() <= rBombBound.right	&& v.Y() >= rBombBound.top && v.Y() <= rBombBound.bottom)
		{
			static_cast<CMob*>(p)->bKnockback = true;
			static_cast<CMob*>(p)->Modify_HP(-1);
		}
	}
}

void CPlayerState::Dir_ByCursor()
{
	POINT pCursor = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	Vector2 vCursorPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)pCursor.x, (float)pCursor.y });
	Vector2 vPlayerPos = pObj->Get_Transform()->Position();

	Vector2 vDiff = vCursorPos - vPlayerPos;

	float fAlpha = 150.f;
	if (vDiff.X() > 0) // 커서가 플레이어의 오른쪽
	{
		if (vDiff.Y() < 0)
		{
			if (vDiff.X() < fAlpha) eDir = D_UP;
			else 	eDir = D_UR;
		}
		else
		{
			if (vDiff.X() < fAlpha) 	eDir = D_DOWN;
			else 	eDir = D_DR;
		}
	}
	else 
	{
		if (vDiff.Y() < 0)
		{
			if (vDiff.X() > -fAlpha)  	eDir = D_UP;
			else  	eDir = D_UL;
		}
		else
		{
			if (vDiff.X() > -fAlpha) 	eDir = D_DOWN;
			else 		eDir = D_DL;
		}
	}
}

void CPlayerState::Shot_Bullet()
{
	if (!bCanShotGun && bCanShotBomb && !bDodgePlaying)
	{
		Attack_Bomb();
	}

	if (!bCanShotGun || bDodgePlaying || bReloading)
		return;

	static_cast<CPlayer*>(pObj)->pWeapon->Attack();
}
