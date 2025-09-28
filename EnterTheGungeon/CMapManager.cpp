#pragma region INCLUDE
#include "pch.h"
#include "CMapManager.h"
#include "CMap.h"
#include "CMapGround.h"

#include "CInputManager.h"
#include "CCollisionManager.h"
#include "CObjectFactory.h"
#include "CCameraManager.h"
#include "CSceneManager.h"
#include "CRelease.h"

#include "CButton.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRenderer.h"

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
	Release();

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
		POINT p = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
		Vector2 realPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)p.x, (float)p.y });
		bDragging = false;
		tempRectList.push_back({ pStart.x, pStart.y, pCurrent.x, pCurrent.y });
	}
}

void CEnvironmentManager::Render(HDC _hDC)
{
	m_hDC = _hDC;
	for (auto& pMap : pCurGroundCollider)
	{
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
		POINT p = MANAGER(CInputManager*, M_INPUT)->Get_CursorPosition();
		Vector2 realPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ (float)p.x, (float)p.y });
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
	for (auto& pMap : pCurGroundCollider)
	{
		CRelease<CMap*>::Release(pMap); 
	}
	pCurGroundCollider.clear();
	
	MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_INTERACTABLE)->clear();

	//pCurGroundCollider.erase(
	//	remove_if(pCurGroundCollider.begin(), pCurGroundCollider.end(),
	//		[&](CMap* pMap)
	//		{
	//			if (pMap->Get_ObjType() == O_MAP)
	//			{
	//				CRelease<CMap*>::Release(pMap);
	//				return true;
	//			}
	//			return false;
	//		}),
	//	pCurGroundCollider.end()
	//);
}


void CEnvironmentManager::Save_Data()
{
	CObject* pPlayer = MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front();

	int iNum = (int)MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene();

	wstring filePath = L"../Data/Ground" + to_wstring(iNum) + L".json";

	std::wofstream ofs(filePath);
	if (!ofs.is_open())
	{
		_tprintf(_T("Save Failed\n"));
		return;
	}

	ofs << L"[\n";
	for (auto iter = pCurGroundCollider.begin(); iter != pCurGroundCollider.end();)
	{
		CCollider c = *(*iter)->Get_Collider();

		ofs << L"  { "
			<< L"\"position X\": " << (int)(*iter)->Get_Transform()->Position().X() << L", "
			<< L"\"position Y\": " << (int)(*iter)->Get_Transform()->Position().Y() << L", "
			<< L"\"left\": " << c.Left() << L", "
			<< L"\"top\": " << c.Top() << L", "
			<< L"\"right\": " << c.Right() << L", "
			<< L"\"bottom\": " << c.Bottom()
			<< L" }";

		iter++;

		if (iter != pCurGroundCollider.end())
			ofs << L",";

		ofs << L"\n";
	}
	ofs << L"]\n";

	ofs.close();

}

void CEnvironmentManager::Load_Data()
{
	int iNum = (int)MANAGER(CSceneManager*, M_SCENE)->Get_CurrentScene();
	
	wstring filePath = L"../Data/Ground" + to_wstring(iNum) + L".json";
	
	wifstream ifs(filePath);
	if (!ifs.is_open())
	{
		_tprintf(_T("Load Failed\n"));
		return;
	}

	std::wstring line;
	while (std::getline(ifs, line))
	{
		int posX, posY, left, top, right, bottom;
		if (swscanf_s(line.c_str(),
			L" { \"position X\": %d, \"position Y\": %d, \"left\": %d , \"top\": %d , \"right\": %d , \"bottom\": %d }",
			&posX, &posY, &left, &top, &right, &bottom) == 6)
		{
			RECT r{ left, top, right, bottom };
			CMapGround* pMap = new CMapGround;
			pMap->Initialize();

			pMap->Get_Transform()->Position({ (float)posX, (float)posY });

			pMap->Get_Collider()->Left(left);
			pMap->Get_Collider()->Top(top);
			pMap->Get_Collider()->Right(right);
			pMap->Get_Collider()->Bottom(bottom);
			pMap->Get_Collider()->Size({ (float)(right - left), (float)(bottom - top) });

			pCurGroundCollider.push_back(pMap);
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
	{
		curMode = None;
		MANAGER(CCameraManager*, M_CAMERA)->Set_CamerMode(CCameraManager::Chase_Player);
	}
	else
	{
		curMode = Ground;
		MANAGER(CCameraManager*, M_CAMERA)->Set_CamerMode(CCameraManager::Edit);
	}
}

void CEnvironmentManager::OnClickSaveButton()
{
	CObject* pPlayer = MANAGER(CObjectManager*, M_OBJECT)->Get_Object(O_PLAYER)->front();
	if (curMode == Ground)
	{
		for (auto& r : tempRectList)
		{
			CMapGround* pMap = new CMapGround;
			pMap->Initialize();

			Vector2 realPos = MANAGER(CCameraManager*, M_CAMERA)->Get_RealPos({ r.left + (r.right - r.left) * 0.5f, r.top + (r.bottom - r.top) *    0.5f });
			Vector2 colSize = { (float)(r.right - r.left), (float)(r.bottom - r.top) };

			pMap->Get_Transform()->Position(move(realPos));

			pMap->Get_Collider()->Left(realPos.X() - colSize.X() * 0.5f);
			pMap->Get_Collider()->Top(realPos.Y() - colSize.Y() * 0.5f);
			pMap->Get_Collider()->Right(realPos.X() + colSize.X() * 0.5f);
			pMap->Get_Collider()->Bottom(realPos.Y() + colSize.Y() * 0.5f);

			pMap->Get_Renderer()->Size({ (float)(r.right - r.left), (float)(r.bottom - r.top) });

			pCurGroundCollider.push_back(pMap);
		}
		tempRectList.clear();
	}

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
