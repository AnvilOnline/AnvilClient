#pragma once
#include <string>

namespace Anvil
{
	extern std::string g_BuildInfo;
	extern void* g_ThreadStorage;

	void* Internal_GetThreadStorage();

	template<typename T>
	T* GetThreadStorage(size_t p_Offset);
}