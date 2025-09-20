#include "pch.h"
#include "CWeapon.h"
#include "CBmpManager.h"
#include "CTransform.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CBullet.h"

CWeapon::CWeapon(CObject* pObj)
{
	pOwner = pObj;
}

CWeapon::~CWeapon()
{
	Release();
}

void CWeapon::Load_Resource()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Weapon/Player_Weapon01.bmp", L"Player_Weapon01");

}

void CWeapon::Initialize()
{
}

int CWeapon::Update()
{
    return 0;
}

void CWeapon::Late_Update()
{
}

void CWeapon::Render(HDC _hDC)
{
}

void CWeapon::Release()
{
}

void CWeapon::Update_Transform()
{
}

void CWeapon::Attack()
{

}