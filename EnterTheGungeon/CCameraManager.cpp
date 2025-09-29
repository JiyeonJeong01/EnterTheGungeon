#include "pch.h"
#include "CCameraManager.h"
#include "CObject.h"
#include "CTransform.h"
#include "CInputManager.h"

CCameraManager::CCameraManager()
{
	vLookAt = {};
	vPrevLookAt = {};
	vCursorPos = {};
	pTarget = nullptr;

	eMode = Chase_Player;

	iShake = 3;
	iShake2 = 8;
	iCurShake = 0;

	fTransitTime = 2.f;
	fDelayTime = 2.f;
	vNewTargetPos = {};
	fMoveDist = 0.f;
	vTransitDir = {};
	bTransit = false;
	bDelay = false;
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::Update()
{
	if (pTarget)
	{
		POINT pScreenCursor = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();

		Vector2 vTargetPos = pTarget->Get_Transform()->Position();
		Vector2 vCursorPos = { pScreenCursor.x + vDiff.X(),  pScreenCursor.y + vDiff.Y() };

		vLookAt= vTargetPos - (vTargetPos - vCursorPos) * 0.2f;
	}

	if (eMode == CameraMode::Chase_Player)
	{
		vCurLookAt = vLookAt;
	}
	else if (eMode == CameraMode::Shake)
	{
		Shake_Camera();
	}
	else if (eMode == CameraMode::Shake2)
	{
		Shake_Camera_2();
	}
	else if (eMode == CameraMode::Transit_NewTarget)
	{
		Transit_Target();
	}
	else if (eMode == CameraMode::Edit)
	{
		Edit_Collider();
	}
	Calculate_Diff();
}

void CCameraManager::Calculate_Diff()
{
	Vector2 vCenter{ WINCX >> 1, WINCY >> 1 };

	vDiff = vCurLookAt - vCenter;
}

void CCameraManager::Set_CamerMode(CameraMode eMode, CObject* pNewTarget)
{
	this->eMode = eMode;

	switch (eMode)
	{
	case CCameraManager::Chase_Player:
	{
	}
	break;
	case CCameraManager::Transit_NewTarget:
	{
		vNewTargetPos = pNewTarget->Get_Transform()->Position();
		vTransitDir = (vNewTargetPos - vCurLookAt);
		float fTotalDist = vTransitDir.Get_Magnitude();
		vTransitDir.Normalize();
		fMoveDist = (fTotalDist / fTransitTime) * 0.015;
		dwTransitElaspedTime = GetTickCount();
		bTransit = false;
		bDelay = false;
		bBack = false;
	}
	break;
	case CCameraManager::Shake: case CCameraManager::Shake2:
	{
		iCurShake = 0;
	}
	break;
	case CCameraManager::Edit:
	{
		editPosition = vCurLookAt;
	}
	}
}

void CCameraManager::Transit_Target()
{
	DWORD d = GetTickCount();
	if (!bTransit && dwTransitElaspedTime + fTransitTime * 1000 > GetTickCount())
	{
		vCurLookAt += vTransitDir * fMoveDist;
		return;
	}
	else if (!bDelay)
	{
		dwTransitElaspedTime = GetTickCount();
		bTransit = true;
		bDelay = true;
	}

	if (dwTransitElaspedTime + fDelayTime * 1000 > GetTickCount() && !bBack)
	{
		vCurLookAt = vNewTargetPos;
		return;
	}
	else if (!bBack)
	{
		bBack = true;
		dwTransitElaspedTime = GetTickCount();
	}

	if (bBack)
	{
		if (dwTransitElaspedTime + fTransitTime * 1000 > GetTickCount())
		{
			vCurLookAt -= vTransitDir * fMoveDist;
		}
		else
		{
			eMode = Chase_Player;
		}
	}
}

void CCameraManager::Shake_Camera_2()
{
	static const float offsets[20] = {
		50.f, -40.f, 35.f, -30.f,
		25.f, -22.f, 18.f, -15.f,
		12.f, -10.f, 8.f, -6.f,
		5.f, -4.f, 3.f, -2.f,
		1.5f, -1.f, 0.5f, -0.2f
	};

	if (iCurShake < 20)
	{
		float offset = offsets[iCurShake];
		vCurLookAt = { vLookAt.X() + offset, vLookAt.Y() - offset };
		iCurShake++;
	}
	else
	{
		eMode = Chase_Player;
		iCurShake = 0;
	}
}

void CCameraManager::Shake_Camera()
{
	if (iCurShake == 0)
	{
		vCurLookAt = { vLookAt.X() - 7.f, vLookAt.Y() + 7.f };
		iCurShake++;
	}
	else if (iCurShake == 1)
	{
		vCurLookAt = { vLookAt.X() + 7.f, vLookAt.Y() - 7.f };
		iCurShake++;
	}
	else if (iCurShake == 2)
	{
		vCurLookAt = { vLookAt.X() - 3.f, vLookAt.Y() + 3.f };
		iCurShake++;
	}
	else if (iCurShake == iShake)
	{
		eMode = Chase_Player;
		iCurShake = 0;
	}
}

void CCameraManager::Edit_Collider()
{
	vCurLookAt =  editPosition;
}
