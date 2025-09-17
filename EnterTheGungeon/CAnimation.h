#pragma once
class CObject;
class CState;

class CAnimation
{
public:
	CAnimation();
	~CAnimation();
public:
	void Initialize(int _iStartIndex, int _iEndIndex, int _iDirRow);
public:
	int iStartIndex;
	int iEndIndex;
	int iDirRow;
	int iCurrIndex;

	DWORD dwLastPlayTime;

	vector<DWORD> vTransitTime;
};