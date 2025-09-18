#pragma once
#include "CUIComponent.h"
class CPointerHandler :  public CUIComponent
{
public:
	CPointerHandler(CObject* pObj);
	~CPointerHandler() override;

public:
	void Initialize();
	void Update_PointerState(); // It must be called update every frame

	bool Add_OnBeginHover() { return !bPrevInRange && bCurrInRange; }
	bool On_Hover() { return bPrevInRange && bCurrInRange; }
	bool On_End_Hover() { return bPrevInRange && !bCurrInRange; }

	bool Add_OnBeginClick() { return !bPrevClick && bCurrClick; }
	bool On_Click() { return bPrevClick && bCurrClick; }
	bool On_End_Click() { return bPrevClick && !bCurrClick; }

private:
	RECT* rDetectRange;

	bool bPrevInRange;
	bool bCurrInRange;

	bool bPrevClick;
	bool bCurrClick;
};

