#pragma once
#include "CMapCollider.h"
class CMapGroundCollider : public CMapCollider
{
public:
	CMapGroundCollider();
	~CMapGroundCollider() override;

public:
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

public:
	void OnCollision(CObject* pObj);
	
};

