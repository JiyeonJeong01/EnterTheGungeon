#pragma once
#include "CComponent.h"
class CObject;

class CTransform : public CComponent
{
public:
	CTransform(CObject* _pObj) : CComponent(_pObj) {}
	~CTransform() override {}

public:
	Vector2 Position() const { return vPosition; }
	void Position(Vector2&& _vPosition) { vPosition.X(_vPosition.X()); vPosition.Y(_vPosition.Y()); }

	Vector2 Size() const { return vSize; }
	void Size(Vector2&& _vSize) { vSize.X(_vSize.X()); vSize.Y(_vSize.Y()); }

	Vector2 Direction() const { return vDir; }
	void Direction(Vector2&& _vDir) { vDir.X(_vDir.X()); vDir.Y(_vDir.Y()); }

private:
	Vector2 vPosition;
	Vector2 vSize;
	Vector2 vDir; // Always normalized
};

