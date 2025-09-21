#pragma once
#include "CMap.h"
class CMapGround : public CMap
{
public:
	CMapGround();
	~CMapGround() override;

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

public:
	void OnCollision(CObject* pObj, Vector2 vDiff) override;
	void OnCollision_Entity(CObject* pObj, Vector2 vDiff) override;
};

