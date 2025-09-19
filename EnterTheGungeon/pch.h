#ifndef PCH_H
#define PCH_H

#include "framework.h"

#include "Define.h"
#include <algorithm>
#include <functional>
#pragma region Container
#include <vector>
#include <list>
#include <map>
#pragma endregion

#pragma region WindowsAPI
#include <Windows.h>
#include <wingdi.h>
#pragma endregion

#pragma region Debug
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW
#endif
#endif // !DBG_NEW
#pragma endregion

#pragma region USERDEFINE
#include "Vector2.h"

#pragma endregion


using namespace std;

#endif //PCH_H
