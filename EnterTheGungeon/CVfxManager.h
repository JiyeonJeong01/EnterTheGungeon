#pragma once
#include "CManager.h"
#include "CVfx.h"
class CVfxManager :   public CManager
{
public :
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Release() override;

public :
	void Play_Vfx(Vector2 vPos, bool& bEnd);

private :
	map<const TCHAR*, CVfx> currentVfxMap;

};

