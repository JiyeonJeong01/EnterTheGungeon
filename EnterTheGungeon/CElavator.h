#pragma once
#include "CObject.h"
class CElavator :   public CObject
{
public :
	enum ElavatorState { Rising, Opening, Opened };
public :
	void Initialize() override;
	int Update() override;
	void Render(HDC hDC) override;
	void Late_Update() override;
	void Release() override;

public :
	void Update_AnimFrame();

private :
	void Rising_Logic();
	void Opening_Logic();
	void Opened_Logic();

private :
	ElavatorState eState;
	DWORD dwElavatorElapsedTime;

	int iRealSizeX, iRealSizeY, iRenderSizeX, iRenderSizeY;
	int iAnimCol, iAnimRow, iMaxAnimCol;

	DWORD dwAnimFrameElapsedTime;
	float fFrameRange;
};

