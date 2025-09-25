#pragma once

#include "CMapObject.h"

class CPlayer;
class CTeleport : public CMapObject
{
public:
	CTeleport();
	~CTeleport() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

public:
	void Update_Collider() override;

public:
	void OnCollision(CObject* pObj, Vector2 vDiff) override;
	void OnCollision_Entity(CObject* pObj, Vector2 vDiff) override;

private :
	CPlayer* pPlayer;
	RECT rDetectBound;

	const TCHAR* animKey;
	int iAnimCol;

	DWORD dwAnimElapsedTime;
	float fAnimTransit;
	
};

