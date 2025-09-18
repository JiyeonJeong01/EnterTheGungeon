#pragma once
#include "CManager.h"

class CObject;

class CCameraManager :  public CManager
{
public:
	CCameraManager();
	~CCameraManager(); 

public:
	void Update();
	void Calculate_Diff();

public :
	void Set_LookAt(Vector2 vLook) { vLookAtPos = vLook;  }
	void Set_Target(CObject* pObj) { pTarget = pObj; }


private :
	Vector2 vLookAtPos;
	CObject* pTarget;
};

