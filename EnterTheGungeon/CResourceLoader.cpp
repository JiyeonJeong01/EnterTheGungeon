#include "pch.h"
#include "CResourceLoader.h"

#include "CWeapon.h"
#include "CBullet.h"
#include "CMap.h"
#include "CItem.h"
#include "CPlayerInfo.h"

#include "CBmpManager.h"



void CResourceLoader::Load_Resource()
{
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Back/Back.bmp", L"Back");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Back/Tutorial.bmp", L"Tutorial");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Map/Stage01.bmp", L"Stage01");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Map/Stage02.bmp", L"Stage02");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Map/Stage03.bmp", L"Stage03");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Map/Store.bmp", L"Store");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Boss_HPBar.bmp", L"Boss_HPBar");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Boss_HP_Effect.bmp", L"Boss_HP_Effect");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Teleport_Opened.bmp", L"Teleport_Opened");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Teleport_Active.bmp", L"Teleport_Active");
	
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Info.bmp", L"Info");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Popup.bmp", L"Popup");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/Dialogue.bmp", L"Dialogue");

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/EKey.bmp", L"EKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/WKey.bmp", L"WKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/AKey.bmp", L"AKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/SKey.bmp", L"SKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/DKey.bmp", L"DKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/QKey.bmp", L"QKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/RKey.bmp", L"RKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/CtrlKey.bmp", L"CtrlKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/SpaceKey.bmp", L"SpaceKey");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/LeftClick.bmp", L"LeftClick");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/RightClick.bmp", L"RightClick");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/LastChest.bmp", L"LastChest");

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/WeaponType_Active.bmp", L"WeaponType_Active");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Objects/Logo.bmp", L"Jusin");


	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Bomb_Effect.bmp", L"Bomb_Effect");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Bullet_R01.bmp", L"Bullet_R01");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Bullet_R02.bmp", L"Bullet_R02");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/BulletEffect06.bmp", L"BulletEffect06");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/BulletEffect07.bmp", L"BulletEffect07");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Cartridge_Effect.bmp", L"Cartridge_Effect");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Heal.bmp", L"Heal");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Hit.bmp", L"Hit");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Shot.bmp", L"Shot");
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Vfx/Teleport.bmp", L"TeleportEffect");

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster_Spawn.bmp", L"Monster_Spawn");



	CWeapon::Load_Resource();
	CBullet::Load_Resource();
	CMap::Load_Resource();
	CItem::Load_Resource();
	CPlayerInfo::Load_Resource();

	if (AddFontResourceEx(_T("../Fonts/Galmuri9.ttf"), FR_PRIVATE, 0) == 0)
	{
		MessageBox(nullptr, _T("폰트 로드 실패!"), _T("Error"), MB_OK);
	}
}