#pragma region INCLUDE
#include "pch.h"
#include "CMob01WalkState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob01.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#pragma endregion


CMob01WalkState::CMob01WalkState(CObject* pObj, CStateMachine* pStateMachine)
    : CMobState(pObj, pStateMachine)
{
}

void CMob01WalkState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 0.8f;
	fCurStateMaxTime = 5.f;
	dwLastAttackTime = GetTickCount();
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster01_WALK.bmp", L"Monster01_WALK");
	animation.Initialize(0, 5, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 100);
}

void CMob01WalkState::Update()
{
	CMobState::Update();

	Update_AnimFrame();
	if (fCurDistToPlayer >= fMinStopDist && fCurDistToPlayer <= fMaxStopDist)
	{
		pStateMachine->Change_State((int)CMob::Idle);
	}
	else if (bInProperDist)
	{
		pObj->Get_Transform()->Direction(move(vDirToPlayer));
	}
	else if (fCurDistToPlayer <= fMinDistToPlayer)
	{
		pObj->Get_Transform()->Direction(move(vDirToPlayer * -1.f));
	}

	printf("CMob01WalkState -> Dist to player: %d\n", (int)fCurDistToPlayer);
	printf("CMob01WalkState -> Should Attack : %d\n", bShouldAttack);
}

void CMob01WalkState::Late_Update()
{
	if (dwLastAttackTime + fAttackTimeRange * 1000 < GetTickCount())
	{
		dwLastAttackTime = GetTickCount();
		Do_Attack();
	}
}

void CMob01WalkState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster01_WALK");

	CRenderer renderer = *(pObj->Get_Renderer());
	int size = 220;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		80, 80,
		hMemDC,
		animation.iCurrIndex * size,
		Dir_AnimRow(eDir) * size,
		size, size,
		RGB(255, 0, 255));
}

void CMob01WalkState::Release()
{
}

void CMob01WalkState::Exit()
{
}

void CMob01WalkState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
}

void CMob01WalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob01WalkState::Stop_Animation()
{
}

void CMob01WalkState::On_End_Animation()
{
}

int CMob01WalkState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: 	case D_UR:	case D_UL:
		return 0;
	case D_DOWN:
		return 1;
	case D_LEFT: case D_DL:
		return 2;
	case D_RIGHT:case D_DR:
		return 3;
	case D_END:
		break;
	default:
		break;
	}
    return 0;
}

void CMob01WalkState::Do_Attack()
{
	CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
		O_ENBULLET,
		pObj->Get_Transform()->Position().X() + vDirToPlayer.X() * 5.f,
		pObj->Get_Transform()->Position().Y() + vDirToPlayer.Y() * 5.f
	));
	pBullet->Set_BulletType(CBullet::B02);
	pBullet->Set_EffectType(CBullet::E03);
	pBullet->Apply_BulletSprite();
	pBullet->Apply_EffectAnim();

	pBullet->Set_Direction(vDirToPlayer);
	pBullet->Set_Speed(5.f);
}

void CMob01WalkState::Do_KnockBack()
{
}
