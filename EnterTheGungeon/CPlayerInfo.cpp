#include "pch.h"
#include "CPlayerInfo.h"
#include "CBmpManager.h"
#include "CPlayer.h"
#include "CCameraManager.h"
#include "CRenderer.h"
#include "CCollider.h"
#include "CTransform.h"

CPlayerInfo::CPlayerInfo()
{
}

CPlayerInfo::~CPlayerInfo()
{
	Release();
}

void CPlayerInfo::Initialize()
{
	CObject::Initialize();
	pRenderer->rType = RND__UI;

	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/UI/HP.bmp", L"HP");

	pTransform->Size({ 35.f, 35.f });
	pCollider->Size({ 35.f, 35.f });
	pRenderer->Size({ 35.f, 35.f });

}

int CPlayerInfo::Update()
{
	return 0;
}

void CPlayerInfo::Late_Update()
{
}

void CPlayerInfo::Render(HDC hDC)
{
	HDC hHPDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"HP");

	GdiTransparentBlt(hDC,
		0, 0,
		35, 35,
		hHPDC,
		0, 0,
		35, 35,
		RGB(255, 0, 255));
}

void CPlayerInfo::Release()
{
}

void CPlayerInfo::Update_Renderer()
{
}
