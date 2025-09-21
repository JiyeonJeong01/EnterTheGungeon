#pragma once
#include "CObject.h"

class CMap : public CObject
{
public:
	CMap();
	~CMap() override;

public :
	static void Load_Resource();
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

public:
	//virtual void Set_Position(Vector2 vPos) { pTransform->Position(move(vPos)); }

public :
	virtual void OnCollision(CObject* pObj, Vector2 vDiff);
	virtual void OnCollision_Entity(CObject* pObj, Vector2 vDiff);

public :
	void Add_OnCollision(function<void(CObject*, Vector2)> listener) { onCollision = listener; }
	function<void(CObject*, Vector2)>* Get_OnCollision() { return &onCollision; }

private :
	function<void(CObject*, Vector2)> onCollision;
};

