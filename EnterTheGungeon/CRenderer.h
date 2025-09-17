#pragma once
#include "CComponent.h"
class CObject;

class CRenderer : public CComponent
{
public:
	CRenderer(CObject* _pObj) : CComponent(_pObj), rRenderer({ 0, 0, 0, 0 }) {}
	~CRenderer() override {}

public:
	int Left() const { return (int)rRenderer.left; }
	int Top() const { return (int)rRenderer.top; }
	int Right() const { return (int)rRenderer.right; }
	int Bottom() const { return (int)rRenderer.bottom; }

	void Left(int _value) { rRenderer.left = _value; }
	void Top(int _value) { rRenderer.top = _value; }
	void Right(int _value) { rRenderer.right = _value; }
	void Bottom(int _value) { rRenderer.bottom = _value; }

public :
	RenderType rType;

private:
	RECT rRenderer;
};