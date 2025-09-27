#include "pch.h"
#include "CBomb.h"

#include "CTransform.h"
#include "CRenderer.h"
#include"CCollider.h"

void CBomb::Late_Initialize()
{
	pTransform->Size({ 40, 40 });
	pCollider->Size({ 40, 40 });
	pRenderer->Size({ 40, 40 });
	iRealSizeX = iRealSizeY = 40;

	iAnimSizeX = iAnimSizeY = (int)pRenderer->Size().X();
	spriteKey = L"Bomb";

	rDetectBound = { -40, -40, 40, 40 };
}

void CBomb::Get_Item()
{
	CItem::Get_Item();
}

void CBomb::Apply_ItemEffect()
{
	printf("used\n");
}
