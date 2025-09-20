#pragma once
#include "CManager.h"

class CMapCollider;
class CMapGroundCollider;
class CMapObjCollider;
class CButton;

class CEnvironmentManager : public CManager
{
public :
	enum EditMode { Ground, Object, None };

public :
	CEnvironmentManager() {};
	~CEnvironmentManager() override;

public :
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC);
	void Release() override;

public :
	void Save_Data();
	void Load_Data();

public :
	void OnClickStartButton();
	void OnClickSaveButton();
	void OnClickClearButton();
	void OnClickTransModeButton();

public :
	list<CMapGroundCollider*>* Get_MapGroundList() { return &pCurGroundCollider; }
	list<CMapObjCollider*>* Get_MapObjList() { return &pCurObjCollider; }

public :
	CButton* pStartEditButton;
	CButton* pSaveButton;
	CButton* pClearButton;
	CButton* pTransEditButton;

	list<RECT> tempRectList;
	list<CMapCollider*> pCurMapCollider;
	list< CMapGroundCollider*> pCurGroundCollider;
	list<CMapObjCollider*> pCurObjCollider;

private :
	bool bEdit;
	bool bDragging;
	EditMode curMode;

	POINT pStart;
	POINT pCurrent;

	HDC m_hDC;
};

