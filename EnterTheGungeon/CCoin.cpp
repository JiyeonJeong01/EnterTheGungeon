#include "pch.h"
#include "CCoin.h"

#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

int CCoin::Update()
{
	CItem::Update();

	if (bChase && !bObtained)
	{
		Chase_Player();
	}

	return 0;
}

void CCoin::OnDetect_Player()
{
	bChase = true;
}

/// <summary>
/// Derived objects must initialize rDetectBound, spriteKey, iAnimSizeX, iAnimSizeY, transform, renderer;
/// </summary>
void CCoin::Late_Initialize()
{
	pTransform->Size({ 40, 40 });
	pCollider->Size({ 40, 40 });
	pRenderer->Size({ 40, 40 });

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Coin";

	rDetectBound = { -150, -150, 150, 150 };

	bChase = false;
	fSpeed = 6.f;
}


void CCoin::Get_Item()
{
	// TODO : pplayer의 인벤토리 받아오기!!!! 
	bObtained = true;
	pPlayer->pInventory->Set_Coin(pPlayer->pInventory->Get_Coin() + 10);
}

void CCoin::Apply_ItemEffect()
{

}

void CCoin::Chase_Player()
{
	Vector2 playerPos = pPlayer->Get_Transform()->Position();
	Vector2 coinPos = pTransform->Position();
	Vector2 vDiff = playerPos - coinPos;
	
	float fDist = sqrtf(vDiff.X() * vDiff.X() + vDiff.Y() * vDiff.Y());

	if (fDist < 20.f)
	{
		Get_Item();
	}

	vDiff.Normalize();
	pTransform->Position({ coinPos.X() + vDiff.X() * fSpeed, coinPos.Y() + vDiff.Y() * fSpeed });
}
