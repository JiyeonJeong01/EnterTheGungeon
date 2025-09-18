#pragma once
#include "CManager.h"
#include "CObjectManager.h"
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

		MANAGER(CObjectManager*, M_OBJECT)->Add_Object(eObject, pObj);

		return pObj;
	}

	static CObject* Create(ObjectType eObject, float fX, float fY)
	{
		CObject* pObj = new T;
		pObj->Initialize();
		pObj->Get_Transform()->Position({ fX, fY });

		MANAGER(CObjectManager*, M_OBJECT)->Add_Object(eObject, pObj);

		return pObj;
	}
	
	
};

