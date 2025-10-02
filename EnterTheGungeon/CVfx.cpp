#include "pch.h"
#include "CVfx.h"

CVfx::CVfx()
{
	iStartIndex = 0;
	iEndIndex = 0;
	iCurIndex = 0;
	bIsEnd = false;
	dwVfxElapsedTime = GetTickCount();
}

CVfx::~CVfx()
{
}