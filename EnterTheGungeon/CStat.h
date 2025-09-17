#pragma once
#include "CComponent.h"
class CStat : public CComponent
{
public :
	CStat(CObject* pObj) : CComponent(pObj) {}
	~CStat() override {}

public :
	float Get_CurrentValue() { return fCurrentValue; }
	void Set_CurrentValue(float fNew) { fCurrentValue = fNew; }
	void Update_CurrentValue(float fAlpha) { fCurrentValue += fAlpha; }

	float Get_MaxValue() { return fCurrentValue; }
	void Set_MaxValue(float fNew) { fMaxValue = fNew; }
	void Update_MaxValue(float fAlpha) { fMaxValue += fAlpha; }

	float Set_CurrentToMax() { fCurrentValue = fMaxValue; }

private :
	float fCurrentValue;
	float fMaxValue;
};

