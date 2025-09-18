#pragma once
#include "CMapCollider.h"
class CObject;
class CMapObjCollider :   public CMapCollider
{
public :
	CMapObjCollider() {}
	~CMapObjCollider() override;

public :
	list<function<void(CObject*)>> onCollision;

};

