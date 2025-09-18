#pragma region INCLUDE
#include "pch.h"
#include "CMapManager.h"
#include "CMap.h"
#include "CButton.h"
#include "CInputManager.h"
#include "CCollisionManager.h"
#include "CObjectFactory.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"
#include "CRelease.h"
#pragma endregion

void CEnvironmentManager::Initialize()
{
	bEdit = false;
	Load_Data();

	pStartEditButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 11));
	pStartEditButton->Get_Transform()->Size({ 30.f, 20.f });
	pStartEditButton->Get_Collider()->Size({ 30.f, 20.f });
	pStartEditButton->Get_Renderer()->Size({ 30.f, 20.f });

	pSaveButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 31));
	pSaveButton->Get_Transform()->Size({ 30.f, 20.f });
	pSaveButton->Get_Collider()->Size({ 30.f, 20.f });
	pSaveButton->Get_Renderer()->Size({ 30.f, 20.f });
	pSaveButton->bActive = false;

	pClearButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 51));
	pClearButton->Get_Transform()->Size({ 30.f, 20.f });
	pClearButton->Get_Collider()->Size({ 30.f, 20.f });
	pClearButton->Get_Renderer()->Size({ 30.f, 20.f });
	pClearButton->bActive = false;

	pTransEditButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 71));
	pTransEditButton->Get_Transform()->Size({ 30.f, 20.f });
	pTransEditButton->Get_Collider()->Size({ 30.f, 20.f });
	pTransEditButton->Get_Renderer()->Size({ 30.f, 20.f });
	pTransEditButton->bActive = false;

	pStartEditButton->Add_OnBeginClick([&] {this->OnClickStartButton(); });
	pClearButton->Add_OnBeginClick([&] {this->OnClickClearButton(); });
	pSaveButton->Add_OnBeginClick([&] {this->OnClickSaveButton(); });
	pTransEditButton->Add_OnBeginClick([&] {this->OnClickTransModeButton(); });
}

void CEnvironmentManager::Update()
{
	if (bEdit && bDragging && MANAGER(CInputManager*, M_INPUT)->Get_KeyUp(VK_RBUTTON))
	{
		bDragging = false;
		tempRectList.push_back({ pStart.x, pStart.y, pCurrent.x, pCurrent.y });
	}
}

void CEnvironmentManager::Render(HDC _hDC)
{
	m_hDC = _hDC;

	for (auto& rect : curGroundRectList)
		Rectangle(_hDC, rect.left, rect.top, rect.right, rect.bottom);
	for (auto& rect : curObjectRectList)
		Rectangle(_hDC, rect.left, rect.top, rect.right, rect.bottom);
	for (auto& rect : tempRectList)
		Rectangle(_hDC, rect.left, rect.top, rect.right, rect.bottom);

	if (!bEdit) return;

	if (MANAGER(CInputManager*, M_INPUT)->Get_KeyDown(VK_RBUTTON))
	{
		pStart = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
	}

	if (MANAGER(CInputManager*, M_INPUT)->Get_Key(VK_RBUTTON))
	{
		bDragging = true;
		pCurrent = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
		Rectangle(_hDC, pStart.x, pStart.y, pCurrent.x, pCurrent.y);
	}
}

void CEnvironmentManager::Release()
{
	CRelease<CButton*>::Release(pStartEditButton);
	CRelease<CButton*>::Release(pSaveButton);
	CRelease<CButton*>::Release(pClearButton);
	CRelease<CButton*>::Release(pTransEditButton);
	for_each(pCurCollider.begin(), pCurCollider.end(), [&](CMap* pMap) -> void
	{
		CRelease<CMap*>::Release(pMap);
	});
}

void CEnvironmentManager::Draw_Line()
{
}

void CEnvironmentManager::Save_Data()
{
	HANDLE hFile = CreateFile(L"../Data/Ground.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Save Failed\n"));
		return;
	}

	DWORD dwByte(0);

	for (auto& ground : curGroundRectList)
	{
		WriteFile(hFile, &(ground), sizeof(RECT), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

void CEnvironmentManager::Load_Data()
{
	HANDLE hFile = CreateFile(L"../Data/Ground.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("Load Failed\n"));
		return;
	}

	DWORD dwByte(0);

	RECT r{};

	while (true)
	{
		ReadFile(hFile, &r, sizeof(RECT), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CMap* pMap = new CMap;
		pMap->Set_Collider(r);
		pCurCollider.push_back(pMap);
		curGroundRectList.push_back(r);
	}
}

void CEnvironmentManager::Transit_EditMode()
{

}

void CEnvironmentManager::OnClickStartButton()
{
	bEdit = !bEdit;
	pSaveButton->bActive = bEdit;
	pClearButton->bActive = bEdit;
	pTransEditButton->bActive = bEdit;
}

void CEnvironmentManager::OnClickSaveButton()
{
	if (curMode == Ground)
		for (auto& r : tempRectList)
		{
			curGroundRectList.push_back(r);
			pCurCollider.push_back(new CMap(r));
		}
	else if (curMode == Object)
		for (auto& r : tempRectList) curObjectRectList.push_back(r);

	Save_Data();
}

void CEnvironmentManager::OnClickClearButton()
{
	tempRectList.clear();
}

void CEnvironmentManager::OnClickTransModeButton()
{
	curMode = (EditMode)((curMode + 1) % 3);
}
