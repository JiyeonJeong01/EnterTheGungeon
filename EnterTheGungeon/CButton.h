#pragma once
#include "CObject.h"
#include "CPointerHandler.h"
class CButton : public CObject
{
public:
	CButton();
	virtual ~CButton();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void Update_Transform() override {};
	void Update_Renderer() override;

public:
	void Add_OnBeginClick(function<void()> event) { onBeginClick.push_back(event); }
	void Add_OnBeginHover(function<void()> event) { onBeginHover.push_back(event); }

public :
	RECT* Get_Rect() { return &rButtonRange; }

public :
	bool bActive;

private:
	RECT rButtonRange;

	CPointerHandler* pPointer;

	list<function<void()>> onBeginClick;
	list<function<void()>> onBeginHover;
};

