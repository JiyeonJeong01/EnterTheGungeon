#pragma region INCLUDE
#include "pch.h"
#include "CMapManager.h"
#include "CMapCollider.h"
#include "CButton.h"
#include "CInputManager.h"
#include "CCollisionManager.h"
#include "CObjectFactory.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"
#include "CRelease.h"

#include <fstream>
#include <sstream>
#pragma endregion

 CEnvironmentManager::~CEnvironmentManager()
{
	Release();
}

void CEnvironmentManager::Initialize()
{
	bEdit = false;
	Load_Data();
#pragma region Debugging
	pStartEditButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 11));
	pStartEditButton->Get_Transform()->Size({ 30.f, 20.f });
	pStartEditButton->Get_Collider()->Size({ 30.f, 20.f });
	pStartEditButton->Get_Renderer()->Size({ 30.f, 20.f });
	pStartEditButton->Get_Renderer()->rType = RND__UI;

	pSaveButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 31));
	pSaveButton->Get_Transform()->Size({ 30.f, 20.f });
	pSaveButton->Get_Collider()->Size({ 30.f, 20.f });
	pSaveButton->Get_Renderer()->Size({ 30.f, 20.f });
	pSaveButton->bActive = false;
	pSaveButton->Get_Renderer()->rType = RND__UI;


	pClearButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 51));
	pClearButton->Get_Transform()->Size({ 30.f, 20.f });
	pClearButton->Get_Collider()->Size({ 30.f, 20.f });
	pClearButton->Get_Renderer()->Size({ 30.f, 20.f });
	pClearButton->bActive = false;
	pClearButton->Get_Renderer()->rType = RND__UI;


	pTransEditButton = dynamic_cast<CButton*>(CObjectFactory<CButton>::Create(O_UI, WINCX - 50, 71));
	pTransEditButton->Get_Transform()->Size({ 30.f, 20.f });
	pTransEditButton->Get_Collider()->Size({ 30.f, 20.f });
	pTransEditButton->Get_Renderer()->Size({ 30.f, 20.f });
	pTransEditButton->bActive = false;
	pTransEditButton->Get_Renderer()->rType = RND__UI;


	pStartEditButton->Add_OnBeginClick([&] {this->OnClickStartButton(); });
	pClearButton->Add_OnBeginClick([&] {this->OnClickClearButton(); });
	pSaveButton->Add_OnBeginClick([&] {this->OnClickSaveButton(); });
	pTransEditButton->Add_OnBeginClick([&] {this->OnClickTransModeButton(); });

#pragma endregion
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
	for (auto& pMap : pCurMapCollider)
	{
		RECT rect = pMap->Get_Collider();
		pMap->Render(_hDC);
	}
	for (auto& rect : tempRectList)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, GetStockObject(HOLLOW_BRUSH));
		HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

		Rectangle(_hDC, rect.left, rect.top, rect.right, rect.bottom);

		SelectObject(_hDC, hOldBrush);
		SelectObject(_hDC, hOldPen);
		DeleteObject(hPen);
	}

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
	for_each(pCurMapCollider.begin(), pCurMapCollider.end(), [&](CMapCollider* pMap) -> void
	{
		CRelease<CMapCollider*>::Release(pMap);
	});
}

void CEnvironmentManager::Save_Data()
{
	CObject* pPlayer = MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front();

	std::wofstream ofs(L"../Data/Ground.json");
	if (!ofs.is_open())
	{
		_tprintf(_T("Save Failed\n"));
		return;
	}

	ofs << L"[\n";
	for (auto iter = pCurMapCollider.begin(); iter != pCurMapCollider.end();)
	{
		RECT r = (*iter)->Get_Collider();

		ofs << L"  { "
			<< L"\"left\": " << r.left << L", "
			<< L"\"top\": " << r.top << L", "
			<< L"\"right\": " << r.right << L", "
			<< L"\"bottom\": " << r.bottom
			<< L" }";

		iter++;

		if (iter != pCurMapCollider.end())
			ofs << L",";

		ofs << L"\n";
	}
	ofs << L"]\n";

	ofs.close();

}

void CEnvironmentManager::Load_Data()
{
	std::wifstream ifs(L"../Data/Ground.json");
	if (!ifs.is_open())
	{
		_tprintf(_T("Load Failed\n"));
		return;
	}

	std::wstring line;
	while (std::getline(ifs, line))
	{
		int left, top, right, bottom;
		if (swscanf_s(line.c_str(),
			L" { \"left\": %d , \"top\": %d , \"right\": %d , \"bottom\": %d }",
			&left, &top, &right, &bottom) == 4)
		{
			RECT r{ left, top, right, bottom };
			CMapCollider* pMap = new CMapCollider;
			pMap->Set_Collider(r);
			pCurMapCollider.push_back(pMap);
		}
	}
}

void CEnvironmentManager::OnClickStartButton()
{
	bEdit = !bEdit;
	pSaveButton->bActive = bEdit;
	pClearButton->bActive = bEdit;
	pTransEditButton->bActive = bEdit;

	if (!bEdit)
		curMode = None;
	else
		curMode = Ground;
}

void CEnvironmentManager::OnClickSaveButton()
{
	CObject* pPlayer = MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front();
	if (curMode == Ground)
	{
		for (auto& r : tempRectList)
		{
			CMapCollider* pMap = new CMapCollider;
			pMap->Set_Collider(r);
			pCurMapCollider.push_back(pMap);
			pCurMapCollider.push_back(new CMapCollider(r));
			// _tprintf(_T("excuted\t:\t:%f, %f\n"), pPlayer->Get_Transform()->Position().X(), pPlayer->Get_Transform()->Position().Y());
		}
		tempRectList.clear();
	}
	//else if (curMode == Object)
		//for (auto& r : tempRectList) curObjectRectList.push_back(r);

	Save_Data();
}

void CEnvironmentManager::OnClickClearButton()
{
	tempRectList.clear();
}

void CEnvironmentManager::OnClickTransModeButton()
{
	curMode = (EditMode)((curMode + 1) % 2);
}
