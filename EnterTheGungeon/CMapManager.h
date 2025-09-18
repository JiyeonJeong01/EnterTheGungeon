#pragma once
#include "CManager.h"
class CMap;
class CButton;
class CEnvironmentManager : public CManager
{
public :
	enum EditMode { Ground, Object, None };
public :
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC);
	void Release() override;

public :
	void Draw_Line();
	void Save_Data();
	void Load_Data();

public :
	void Transit_EditMode();
	void OnClickStartButton();
	void OnClickSaveButton();
	void OnClickClearButton();
	void OnClickTransModeButton();

public :
	list<CMap*>* Get_MapList() { return &pCurCollider; }

public :
	CButton* pStartEditButton;
	CButton* pSaveButton;
	CButton* pClearButton;
	CButton* pTransEditButton;

	list<RECT> curGroundRectList;
	list<RECT> curObjectRectList;
	list<RECT> tempRectList;
	list<CMap*> pCurCollider;

private :
	bool bEdit;
	bool bDragging;
	EditMode curMode;

	POINT pStart;
	POINT pCurrent;

	HDC m_hDC;
};

