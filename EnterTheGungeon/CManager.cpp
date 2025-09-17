#include "pch.h"
#include "CManager.h"
#include "CInputManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CBmpManager.h"

CManager* CManager::g_pManagers[MGREND] = { nullptr, };

CManager* CManager::Get_Manager(ManagerType eManager)
{
	if (g_pManagers[eManager] == nullptr)
	{
		switch (eManager)
		{
		case ManagerType::M_INPUT:
			g_pManagers[eManager] = new CInputManager();
			break;
		case ManagerType::M_OBJECT:
			g_pManagers[eManager] = new CObjectManager();
			break;
		case ManagerType::M_SCENE:
			g_pManagers[eManager] = new CSceneManager();
			break;
		case ManagerType::M_BMP:
			g_pManagers[eManager] = new CBmpManager();
			break;
		}
	}

	return g_pManagers[eManager];
}
void CManager::Destroy_Manager(ManagerType eManager)
{
	if (g_pManagers[eManager])
	{
		delete g_pManagers[eManager];
		g_pManagers[eManager] = nullptr;
	}
}

void CManager::Destroy_Manager()
{
	delete g_pManagers;
}
