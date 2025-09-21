#pragma once
#include "CMap.h"
class CPlayer;

class CTableObject :  public CMap
{
public :
	enum TableState { Idle, Interact, Flipping, Flipped, Broken };
public:
	CTableObject();
	~CTableObject() override;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override; // Only used for debugging
	void Release() override;

public : 
	void Update_Collider() override;

public:
	void OnCollision(CObject* pObj, Vector2 vDiff) override;
	void OnCollision_Entity(CObject* pObj, Vector2 vDiff) override;

public :
	void Table_Idle();
	void Table_Interact();
	void Table_Flipping();
	void Table_Flipped();
	void Table_Broken();

	int Get_TableDir();
	int Get_CanMove() { return bCanMove;  }

	void Table_Move(CObject* pObj, Vector2 vDiff);
	void Table_Stable(CObject* pObj, Vector2 vDiff);

	Vector2 Check_DetectBound();

public :
	const RECT& Get_DetecRect() { return rDetectBound; }

	TableState Get_TableState() { return eState; }
	void Set_TableState(TableState eState) { this->eState = eState; }


private :
	CPlayer* pPlayer;
	RECT rDetectBound; // Only detect player
	TableState eState;
	Direction interactDir;

	int iAnimRow;
	int iAnimCol;
	int iAnimEndCol;

	int iDetectSize;
	const TCHAR* animKey;

	DWORD dwFlippingTime;
	int iFlippingFrameTime;

	int iHitCount;
	bool bCanMove;
	bool bNoDamage;

	RECT rColliderOffset;

	CObject* pCollisionBullet;

	bool bInvincible;
	DWORD dwInvicibleTime;
};