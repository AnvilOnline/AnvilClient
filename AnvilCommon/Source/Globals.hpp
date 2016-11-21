#pragma once
#include <string>

namespace AnvilCommon
{
	extern std::string g_BuildInfo;
	extern void* g_ThreadStorage;

	extern void *Internal_GetThreadStorage(const size_t p_Offset = 0);

	template<typename T>
	T* GetThreadStorage(size_t p_Offset)
	{
		if (!g_ThreadStorage)
			return nullptr;

		auto s_Storage = *reinterpret_cast<T**>((char *)g_ThreadStorage + p_Offset);

		return s_Storage;
	}
}