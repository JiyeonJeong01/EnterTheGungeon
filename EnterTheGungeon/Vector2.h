#pragma once
class Vector2
{
public:
	Vector2() : fX(0.f), fY(0.f) {};
	Vector2(float _fX, float _fY) : fX(_fX), fY(_fY) {};
	~Vector2() {};

public:
	Vector2 operator+(const Vector2& rhs) const { return Vector2{ fX + rhs.fX, fY + rhs.fY }; }
	Vector2 operator-(const Vector2& rhs) const { return Vector2{ fX - rhs.fX, fY - rhs.fY }; }
	Vector2 operator*(const float _fValue) const { return Vector2{ fX * _fValue, fY * _fValue }; }
	Vector2 operator/(const float _fValue) const { return Vector2{ fX / _fValue, fY / _fValue }; }
	void operator+=(const Vector2& rhs) {
		this->fX += rhs.fX;
		this->fY += rhs.fY;

	}
	void operator-=(const Vector2& rhs) { fX -= rhs.fX; fY -= rhs.fY; }

public:
	void X(float _fX) { fX = _fX; }
	void Y(float _fY) { fY = _fY; }
	float X() { return fX; }
	float Y() { return fY; }

public:
	float Get_Magnitude();
	Vector2 Get_Normalized();

	void Normalize();

private:
	float fX;
	float fY;
};

