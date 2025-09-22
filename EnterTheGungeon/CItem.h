#pragma once
#include "CObject.h"
#include "CPlayer.h"
#include "CInventory.h"
class CItem : public CObject
{
public:
	static void Load_Resource();

public :
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public :
	void Detect_Player();
	virtual void OnDetect_Player();
	virtual void Late_Initialize();
	virtual void Drop_Item(Vector2 vDropPos);
	virtual void Get_Item();
	virtual void Apply_ItemEffect();

public : 
	bool Get_CanUse() { return bCanUse;  }
	const RECT& Get_DetectBound() { return rDetectBound; }

	const TCHAR* Get_ItemKey() { return spriteKey;  }

protected:
	CPlayer* pPlayer;
	RECT rDetectBound;

	bool bCanUse;
	bool bCanInteract;
	bool bObtained;

	int iAnimCol;
	int iAnimSizeX;
	int iAnimSizeY;

	const TCHAR* spriteKey;
};

