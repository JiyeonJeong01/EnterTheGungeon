#pragma region INCLUDE
#include "pch.h"
#include "CMob03WalkState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob03.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#pragma endregion


CMob03WalkState::CMob03WalkState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob03WalkState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 0.8f;
	fCurStateMaxTime = 5.f;
	dwLastAttackTime = GetTickCount();
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster03_ATTACK.bmp", L"Monster03_ATTACK");

	animation.Initialize(0, 7, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 70);
}

void CMob03WalkState::Update()
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
}

void CMob03WalkState::Late_Update()
{

}

void CMob03WalkState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster03_ATTACK");

	CRenderer renderer = *(pObj->Get_Renderer());
	int realSize = 230;
	int iRenderSize = 230;
	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		iRenderSize, iRenderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		0,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob03WalkState::Release()
{
}

void CMob03WalkState::Exit()
{
}

void CMob03WalkState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
}

void CMob03WalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
	if (animation.iCurrIndex == 5)
	{
		bCanAttack = true;
	}
	if (animation.iCurrIndex == animation.iEndIndex - 1)
	{
		Do_Attack();
	}
}

void CMob03WalkState::Stop_Animation()
{
}

void CMob03WalkState::On_End_Animation()
{
}

int CMob03WalkState::Dir_AnimRow(Direction eDir)
{
	if (vDirToPlayer.X() >= 0) return 0;
	else return 1;
}

void CMob03WalkState::Do_Attack()
{
	if (!bCanAttack) return;
	CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
		O_ENBULLET,
		pObj->Get_Transform()->Position().X() + vDirToPlayer.X() * 5.f,
		pObj->Get_Transform()->Position().Y() + vDirToPlayer.Y() * 10.f
	));
	pBullet->Set_BulletType(CBullet::B02);
	pBullet->Set_EffectType(CBullet::E04);
	pBullet->Apply_BulletSprite();
	pBullet->Apply_EffectAnim();

	pBullet->Set_Direction(vDirToPlayer);
	pBullet->Set_Speed(6.f);

	bCanAttack = false;
}

void CMob03WalkState::Do_KnockBack()
{
}
