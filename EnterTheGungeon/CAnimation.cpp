#include "pch.h"
#include "CAnimation.h"

CAnimation::CAnimation()
	: iStartIndex(0), iEndIndex(0), iCurrIndex(0), iDirRow(0), dwLastPlayTime(0)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Initialize(int _iStartIndex, int _iEndIndex, int _iDirRow)
{
	iStartIndex = _iStartIndex;
	iEndIndex = _iEndIndex;
	iDirRow = _iDirRow;
	iCurrIndex = 0;

	dwLastPlayTime = GetTickCount();

	vTransitTime.resize(iEndIndex);
	fill(vTransitTime.begin(), vTransitTime.end(), 90);
}