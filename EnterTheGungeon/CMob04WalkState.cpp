#pragma region INCLUDE
#include "pch.h"
#include "CMob04WalkState.h"
#include "CBmpManager.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CTransform.h"
#include "CPlayer.h"
#include "CMob04.h"
#include "CStateMachine.h"
#include "CMobBullet.h"
#include "CObjectFactory.h"
#include "CSoundManager.h"
#pragma endregion


CMob04WalkState::CMob04WalkState(CObject* pObj, CStateMachine* pStateMachine)
	: CMobState(pObj, pStateMachine)
{
}

void CMob04WalkState::Initialize()
{
	CMobState::Initialize();
	fAttackTimeRange = 1.8f;
	fCurStateMaxTime = 5.f;
	dwLastAttackTime = GetTickCount();
	dwCurrentStateElapsedTime = GetTickCount();
	eState = CMob::Idle;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Monster04_WALK.bmp", L"Monster04_WALK");
	animation.Initialize(0, 5, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 100);
}

void CMob04WalkState::Update()
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

void CMob04WalkState::Late_Update()
{
	if (dwLastAttackTime + fAttackTimeRange * 1000 < GetTickCount())
	{
		dwLastAttackTime = GetTickCount();
		Do_Attack();
	}
}

void CMob04WalkState::Render(HDC hDC)
{
	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Monster04_WALK");

	CRenderer renderer = *(pObj->Get_Renderer());

	int realSize = 150;
	int renderSize = 110;

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		renderSize, renderSize,
		hMemDC,
		animation.iCurrIndex * realSize,
		Dir_AnimRow(eDir) * realSize,
		realSize, realSize,
		RGB(255, 0, 255));
}

void CMob04WalkState::Release()
{
}

void CMob04WalkState::Exit()
{
}

void CMob04WalkState::Enter()
{
	dwCurrentStateElapsedTime = GetTickCount();
}

void CMob04WalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CMob04WalkState::Stop_Animation()
{
}

void CMob04WalkState::On_End_Animation()
{
}

int CMob04WalkState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: 	case D_UR:	case D_UL:
		return 0;
	default:
		return 1;
	}
}

void CMob04WalkState::Do_Attack()
{
	MANAGER(CSoundManager*, M_SOUND)->PlayFX(L"CombineBomb_Shot_01.wav", 1.f);
	int bulletCount = 15;
	float angleRange = 50.f;
	float angleStep = angleRange / (bulletCount - 1); 
	vDirToPlayer.Normalize();
	Vector2 vBaseDir = vDirToPlayer;

	float startAngle = -angleRange / 2.f;
	for (int i = 0; i < bulletCount; i++)
	{
		float angleDeg = startAngle + i * angleStep;
		float angleRad = angleDeg * (3.14159265f / 180.f);
		float cosA = cosf(angleRad);
		float sinA = sinf(angleRad);

		Vector2 vRotatedDir( vBaseDir.X() * cosA - vBaseDir.Y() * sinA, 	vBaseDir.X() * sinA + vBaseDir.Y() * cosA );

		CMobBullet* pBullet = static_cast<CMobBullet*>(CObjectFactory<CMobBullet>::Create(
			O_ENBULLET, pObj->Get_Transform()->Position().X() + vRotatedDir.X() * 5.f, pObj->Get_Transform()->Position().Y() + vRotatedDir.Y() * 5.f ));

		pBullet->Set_BulletType(CBullet::B04);
		pBullet->Set_EffectType(CBullet::E03);
		pBullet->Apply_BulletSprite();
		pBullet->Apply_EffectAnim();
		pBullet->Set_Direction(vRotatedDir);
		pBullet->Set_Speed(5.f);
	}
}


void CMob04WalkState::Do_KnockBack()
{
}
