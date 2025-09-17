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

private:
	RECT rCollider;
};