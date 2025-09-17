#pragma once
class CManager
{
public:
	CManager() {}
	virtual ~CManager() {}

public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Late_Update() {};
	virtual void Release() {};

#pragma region Singleton
public:
	static CManager* Get_Manager(ManagerType eManager);
public:
	static void Destroy_Manager(ManagerType eManager);
public :
	static void Destroy_Manager();
private:
	static CManager* g_pManagers[MGREND];
#pragma endregion
};
