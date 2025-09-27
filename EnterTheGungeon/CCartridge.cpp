#include "pch.h"
#include "CCartridge.h"

#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

#include "CObjectManager.h"
#include "CRelease.h"

#include "CPlayerBullet.h"
#include "CBossBullet.h"

/// <summary>
/// Derived objects must initialize rDetectBound, spriteKey, iAnimSizeX, iAnimSizeY, transform, renderer;
/// </summary>
void CCartridge::Late_Initialize()
{
	pTransform->Size({ 40, 40 });
	pCollider->Size({ 40, 40 });
	pRenderer->Size({ 40, 40 });

	iRealSizeX = iRealSizeY = 40;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Cartridge";

	rDetectBound = { -40, -40, 40, 40 };
}


void CCartridge::Get_Item()
{
	// TODO : pplayer의 인벤토리 받아오기!!!! 
	CItem::Get_Item();
}

void CCartridge::Apply_ItemEffect()
{
	for (auto& obj : *MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLBULLET))
	{
		static_cast<CPlayerBullet*>(obj)->OnCollision_MapGround();
	}

	for (auto& obj : *MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_ENBULLET))
	{
		static_cast<CBossBullet*>(obj)->OnCollision_MapGround();
	}

}
