#include "pch.h"
#include "CBossWalkState.h"
#include "CBmpManager.h"
#include "CStateMachine.h"
#include "CRenderer.h"
#include "CTransform.h"
void CBossWalkState::Initialize()
{
	CBossState::Initialize();
	eState = CBoss::BS_IDLE;
	MANAGER(CBmpManager*, M_BMP)->Insert_Bmp(L"../Sprites/Enemy/Boss_WALK.bmp", L"Boss_WALK");
	animation.Initialize(0, 6, (int)D_DOWN);
	fill(animation.vTransitTime.begin(), animation.vTransitTime.end(), 90);

	fCurStateMaxTime = 2.f;


}

void CBossWalkState::Update()
{
	CBossState::Update();
	Update_AnimFrame();

	Walk();
}

void CBossWalkState::Late_Update()
{
	if (dwCurrentStateElapsedTime + int(fCurStateMaxTime * 1000) < GetTickCount())
	{
		//pStateMachine->Change_State(CBoss::BS_WALK);
	}
}

void CBossWalkState::Render(HDC hDC)
{

	HDC hMemDC = MANAGER(CBmpManager*, M_BMP)->Find_Image(L"Boss_WALK");

	CRenderer renderer = *(pObj->Get_Renderer());

	GdiTransparentBlt(hDC,
		renderer.Left(),
		renderer.Top(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		hMemDC,
		animation.iCurrIndex * (int)renderer.Size().X(),
		Dir_AnimRow(eDir) * (int)renderer.Size().Y(),
		(int)renderer.Size().X(),
		(int)renderer.Size().Y(),
		RGB(255, 0, 255));
}

void CBossWalkState::Release()
{
}

void CBossWalkState::Exit()
{
}

void CBossWalkState::Enter()
{
}

void CBossWalkState::Update_AnimFrame()
{
	CState::Update_AnimFrame();
}

void CBossWalkState::Stop_Animation()
{
}

void CBossWalkState::On_End_Animation()
{
}

int CBossWalkState::Dir_AnimRow(Direction eDir)
{
	switch (eDir)
	{
	case D_UP: return 0;
	case D_UL: return 1;
	case D_UR: return 2;
	case D_DOWN: return 3;
	case D_LEFT: case D_DL: return 4;
	case D_RIGHT: case D_DR: return 5;
	}
	return 0;
}

void CBossWalkState::Walk()
{
	if (fCurDistToPlayer >= fMinStopDist && fCurDistToPlayer <= fMaxStopDist)
	{
		pStateMachine->Change_State(CBoss::BS_IDLE);
	}
	else if (bInProperDist)
	{
		pObj->Get_Transform()->Direction(move(vDirToPlayer));
	}
	else if (fCurDistToPlayer >= fMaxDistToPlayer)
	{
		pObj->Get_Transform()->Direction(move(vDirToPlayer));
	}
	else if (fCurDistToPlayer <= fMinDistToPlayer)
	{
		pObj->Get_Transform()->Direction(move(vDirToPlayer * -1.f));
	}
}
