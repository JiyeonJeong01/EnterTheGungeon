#ifndef PCH_H
#define PCH_H

#include "framework.h"

#include "Define.h"
#include <algorithm>
#include <functional>
#pragma region Container
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#pragma endregion

#pragma region WindowsAPI
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "msimg32.lib")
#ifdef Image
#undef Image
#endif
#pragma endregion

#include <io.h>
#include <iostream>

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_common.h"

#pragma comment(lib, "fmod_vc.lib")

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
