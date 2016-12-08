#pragma once
#include <string>

namespace AnvilCommon
{
	extern std::string g_BuildInfo;

	extern void *Internal_GetThreadStorage(const size_t p_Offset = 0);

	template<typename T>
	T *GetThreadStorage(const size_t p_Offset = 0)
	{
		return *reinterpret_cast<T **>(Internal_GetThreadStorage(p_Offset));
	}
}