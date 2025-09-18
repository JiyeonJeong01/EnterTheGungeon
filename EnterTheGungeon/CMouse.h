#pragma once
#include "CObject.h"
class CMouse : public CObject
{
public :
	CMouse();
	~CMouse() override;

public :
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hDC) override;
	void Release() {};

	void Update_Transform() override;
};

