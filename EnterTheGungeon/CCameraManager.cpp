#include "pch.h"
#include "CCameraManager.h"
#include "CObject.h"
#include "CTransform.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::Update()
{
	if (pTarget)
	{
		vLookAtPos = pTarget->Get_Transform()->Position();
	}
}

void CCameraManager::Calculate_Diff()
{
}
