#pragma once
#include <string>

namespace AnvilCommon
{
	extern std::string g_BuildInfo;
	extern void* g_ThreadStorage;

	extern void* Internal_GetThreadStorage();

	template<typename T>
	T* GetThreadStorage(size_t p_Offset)
	{
		if (!g_ThreadStorage)
			return nullptr;

		auto s_Storage = *reinterpret_cast<T**>(g_ThreadStorage + p_Offset);

		return s_Storage;
	}
}