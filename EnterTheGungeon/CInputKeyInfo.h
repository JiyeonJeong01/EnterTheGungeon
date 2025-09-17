#pragma once
class CInputKeyInfo
{
public:
	CInputKeyInfo() : bPrevPressed(false), bCurrentPressed(false) {}
	~CInputKeyInfo() {}

public:
	bool Get_KeyDown() { return (!bPrevPressed && bCurrentPressed); }
	bool Get_Key() { return (bPrevPressed && bCurrentPressed); }
	bool Get_KeyUp() { return (bPrevPressed && !bCurrentPressed); }

public:
	bool Get_PrevPressed() const { return bPrevPressed; }
	bool Get_CurrentPressed() const { return bCurrentPressed; }
	void Set_PrevPressed(bool value) { bPrevPressed = value; }
	void Set_CurrentPressed(bool value) { bCurrentPressed = value; }

private:
	bool bPrevPressed;
	bool bCurrentPressed;
};