#pragma once

class CObject;

class CComponent
{
public:
	CComponent(CObject* pObj) : pObject(pObj) {}
	virtual ~CComponent() {}
public:
	CObject* Get_Obj() { return pObject; }
	void Set_Obj(CObject* _pObj) { pObject = _pObj; }

private:
	CObject* pObject;
};

