#pragma once
#include "CManager.h"

class CObject;

class CCameraManager :  public CManager
{
public :
	enum CameraMode { Chase_Player, Transit_NewTarget, Shake, Shake2, Edit };
public:
	CCameraManager();
	~CCameraManager(); 

public:
	void Update();
	void Calculate_Diff();

public :
	void Set_CamerMode(CameraMode eMode, CObject* pNewTarget = nullptr);
	void Transit_Target();
	void Shake_Camera_2();
	void Shake_Camera();
	void Edit_Collider();

public :
	void Set_LookAt(Vector2 vLook) { vLookAt = vLook;  }
	void Set_Target(CObject* pObj) { pTarget = pObj; }

	Vector2 Get_LookAt() { return vCurLookAt;  }

	/// <summary>
	/// Subtracts the distance the camera has moved from the object's position
	/// vObjPos - vDiff
	/// vDiff = vLookAt - vCenter
	/// </summary>
	/// <param name="vObjPos"></param>
	/// <returns></returns>
	Vector2 Get_RenderPos(Vector2 vObjPos) {
		return vObjPos - vDiff;
	}
	/// <summary>
	/// Get objec'ts real world position
	/// </summary>
	/// <param name="vRenderPos"></param>
	/// <returns></returns>
	Vector2 Get_RealPos(Vector2 vRenderPos) { return vRenderPos + vDiff; }

private :
	Vector2 vLookAt;
	Vector2 vCurLookAt;
	Vector2 vPrevLookAt;
	Vector2 vCursorPos;

	Vector2 vDiff; // vLookAt - vScreenCenter
	CObject* pTarget;
	CameraMode eMode;

	int iShake;
	int iShake2;
	int iCurShake;

	float fTransitTime;
	float fDelayTime;
	Vector2 vNewTargetPos;
	Vector2 vTransitDir;
	float fMoveDist;
	DWORD dwTransitElaspedTime;
	bool bTransit;
	bool bDelay;
	bool bBack;

	Vector2 editPosition;
};

