#pragma once
class CVfx
{
public :
	CVfx();
	~CVfx();
public :
	int iStartIndex;
	int iEndIndex;
	int iCurIndex;

public :
	bool bLoop;
	bool bIsEnd;
	int iTransitRange;
	DWORD dwVfxElapsedTime;
};

