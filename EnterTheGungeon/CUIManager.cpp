#include "pch.h"
#include "CUIManager.h"

#include "CObject.h"
#include "CRelease.h"

CUIManager::CUIManager()
{

}

CUIManager::~CUIManager()
{
	Release();
}

void CUIManager::Initialize()
{
}

void CUIManager::Update()
{
	for (auto& ui : uiObjects)
	{
		ui->Update();
	}
}

void CUIManager::Late_Update()
{
	for (auto& ui : uiObjects)
	{
		ui->Late_Update();
	}
}

void CUIManager::Render(HDC hDC)
{
	for (auto& ui : uiObjects)
	{
		ui->Render(hDC);
	}
}

void CUIManager::Release()
{
	for (auto& ui : uiObjects)
	{
		CRelease<CObject*>::Release(ui);
	}
}

void CUIManager::Add_Object(CObject* pObj)
{
	if (pObj == nullptr) return;

	uiObjects.push_back(pObj);
}
