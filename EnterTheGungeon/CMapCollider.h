#pragma once
#include "CObject.h"

class CMapCollider : public CObject
{
public:
	CMapCollider();
	~CMapCollider() override;

public :
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

public :
	void OnCollision(CObject* pObj);
	void OnCollision_Entity(CObject* pObj);

public :
	void Add_OnCollision(function<void(CObject*, Vector2)> listener) { onCollision = listener; }
	function<void(CObject*, Vector2)>* Get_OnCollision() { return &onCollision; }

private :
	function<void(CObject*, Vector2)> onCollision;
};

