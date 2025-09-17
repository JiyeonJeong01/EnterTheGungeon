#pragma once
#include "CManager.h"
class CObject;

class CObjectManager : public  CManager
{
public:
	CObjectManager();
	virtual ~CObjectManager() override;

public: // Core
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC);
	void Release() override;

public:
	bool Add_Object(ObjectType eObject, CObject* pObj);
	bool Remove_Object(ObjectType eObject, CObject* pObj);
	list<CObject*>* Get_Object(ObjectType eObject);

	bool Remov_ObjectAll(ObjectType eObject);

private:
	list<CObject*> m_pObjLists[OBJECTEND];
	list<CObject*> m_pRenderList[RND_END];
};
