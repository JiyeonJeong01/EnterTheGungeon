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
	
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/EKey.bmp", L"EKey");

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