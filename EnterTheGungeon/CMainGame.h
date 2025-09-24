#pragma once

class Vignette;

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public :
	void Compute_FPS();

public:
	int iFPS;
	int iFPSCounter;
private:
	HDC hDC;
	Vignette* pVignette;
private:
	LONGLONG llElapsedTime;
	TCHAR		szFPS[64];
	bool bFrameUpdated;
};
