#pragma once
class CSTLHelper
{
public :
	template <typename T>
	static bool Compare_Key(const TCHAR* pKey, T& pair)
	{
		return !lstrcmp(pKey, pair.first);
	}

	template <typename T>
	static void Delete_Map(T& pair)
	{
		if (pair.second)
		{
			delete pair.second;
			pair.second = nullptr;
		}
	}

};

