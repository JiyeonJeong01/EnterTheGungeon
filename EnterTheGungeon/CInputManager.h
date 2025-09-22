#pragma once
#include  "CManager.h"

class CInputKeyInfo;

class CInputManager : public CManager
{
public:
	CInputManager();
	~CInputManager() override;
public:
	void Initialize() override;
	void Update() override;
	void Release() override;

public:
	bool Get_KeyDown(int iKey);
	bool Get_Key(int iKey);
	bool Get_KeyUp(int iKey);

	void Check_KeyInput();
	void Check_CursorPosition();
	const POINT& Get_CursorPosition();

private:
	vector<int> vKeys { VK_LBUTTON, VK_RBUTTON, VK_LCONTROL, VK_SPACE, VK_LSHIFT,
									'W', 'A', 'S', 'D', 'Q', 'E', 'R', 'M'};
	map<int, CInputKeyInfo*> mKeyInfos;

	POINT pCursor;
};

