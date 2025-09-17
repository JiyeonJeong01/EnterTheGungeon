#pragma once
template<typename T>
class CRelease
{
public :
	static void Release(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
};

