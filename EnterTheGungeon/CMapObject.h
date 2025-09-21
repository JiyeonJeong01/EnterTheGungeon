#pragma once
#include "CMap.h"
class CObject;
class CMapObject : public CMap
{
public:
	CMapObject();
	~CMapObject() override;

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

	void OnCollision(CObject* pObj);
	void OnCollision_Entity(CObject* pObj);
};
