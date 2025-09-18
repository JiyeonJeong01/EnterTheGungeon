#pragma once
#include "CObject.h"
class CUIComponent
{
public:
	CUIComponent(CObject* pObj) : pOwner(pObj) {}
	virtual ~CUIComponent() {}

protected:
	CObject* pOwner;
};

