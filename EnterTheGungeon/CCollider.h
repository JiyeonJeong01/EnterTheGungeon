#pragma once
#include "CComponent.h"
class CObject;

class CCollider : public CComponent
{
public:
	CCollider(CObject* _pObj) : CComponent(_pObj), rCollider({ 0, 0, 0, 0 }) {}
	~CCollider() override {}

public:
	int Left() const { return (int)rCollider.left; }
	int Top() const { return (int)rCollider.top; }
	int Right() const { return (int)rCollider.right; }
	int Bottom() const { return (int)rCollider.bottom; }

	void Left(int _value) { rCollider.left = _value; }
	void Top(int _value) { rCollider.top = _value; }
	void Right(int _value) { rCollider.right = _value; }
	void Bottom(int _value) { rCollider.bottom = _value; }

	RECT* Get_PCollider() { return &rCollider; }

	Vector2 Size() const { return vSize; }
	void Size(Vector2&& _vSize) { vSize.X(_vSize.X()); vSize.Y(_vSize.Y()); }

public :
	void Add_OnCollision(function<void(CObject*, Vector2)> listener) { onCollision.push_back(listener); }
	list <function<void(CObject*, Vector2)>>* Get_OnCollision() { return &onCollision; }

private:
	RECT rCollider;
	Vector2 vSize;
private :
	list <function<void(CObject*, Vector2)>> onCollision;
};