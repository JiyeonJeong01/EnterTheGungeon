#pragma once
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

public:
	int iFPS = 60;

private:
	HDC hDC;

private:
	LONGLONG llElapsedTime;
};
