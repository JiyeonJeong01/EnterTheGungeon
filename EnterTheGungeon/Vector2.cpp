#include "pch.h"
#include "Vector2.h"

float Vector2::Get_Magnitude()
{
	return sqrtf(fX * fX + fY * fY);
}

Vector2 Vector2::Get_Normalized()
{
	float fMagnitude = Get_Magnitude();

	if (fMagnitude <= 0.f) return { 0.f, 0.f };

	float fNormX = fX / fMagnitude;
	float fNormY = fY / fMagnitude;

	Vector2 vNormVec(fNormX, fNormY);

	return vNormVec;
}

void Vector2::Normalize()
{
	float fMagnitude = Get_Magnitude();
	if (fMagnitude <= 0.f)
	{
		fX = 0.f;
		fY = 0.f;
	}
	else
	{
		fX /= fMagnitude;
		fY /= fMagnitude;
	}
}
