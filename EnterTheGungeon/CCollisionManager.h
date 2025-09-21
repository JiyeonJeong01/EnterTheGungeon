#pragma once
#include "CManager.h"
#include "CObject.h"
#include "CMapGround.h"
class CCollisionManager : public CManager
{
public :
	static void Detect_RectCollision(list<CObject*> dstList, list<CObject*> srcList);
	static Vector2 Get_OverlapRect(CObject* dstList, CObject* srcList);
	static void Detect_CircleCollision(list<CObject*> dstList, list<CObject*> srcList);
	static Vector2 Get_OverlapCircle(CObject* dstList, CObject* srcList);

	// object <-> map(ground, wall)
	static void Detect_MapCollision(list<CMap*> mapList, list<CObject*> objList);
};

