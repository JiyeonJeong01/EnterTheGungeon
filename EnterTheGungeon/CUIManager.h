#pragma once
#include "CManager.h"
class CObject;
class CUIManager :  public CManager
{
public :
	CUIManager();
	~CUIManager() override;

public :
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render(HDC _hDC);
	void Release() override;

public:
	void Add_Object(CObject* pObj);
	bool Remove_Object(ObjectType eObject, CObject* pObj);
	list<CObject*>* Get_Object(ObjectType eObject);

public :
	list<CObject*> uiObjects;
};

