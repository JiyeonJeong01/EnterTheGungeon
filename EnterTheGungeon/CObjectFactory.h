#pragma once
#include "CManager.h"
#include "CObjectManager.h"
#include "CUIManager.h"
#include "CObject.h"
#include "CTransform.h"

template <typename T>
class CObjectFactory
{
public :
	static CObject* Create(ObjectType eObject)
	{
		CObject* pObj = new T;
		pObj->Initialize();

		if (eObject == O_UI)
		{
			MANAGER(CUIManager*, M_UI)->Add_Object(pObj);
			return pObj;
		}
		MANAGER(CObjectManager*, M_OBJECT)->Add_Object(eObject, pObj);

		return pObj;
	}

	static CObject* Create(ObjectType eObject, float fX, float fY)
	{
		CObject* pObj = new T;
		pObj->Initialize();
		pObj->Get_Transform()->Position({ fX, fY });

		if (eObject == O_UI)
		{
			MANAGER(CUIManager*, M_UI)->Add_Object(pObj);
			return pObj;
		}

		MANAGER(CObjectManager*, M_OBJECT)->Add_Object(eObject, pObj);

		return pObj;
	}
	
	
};

