#include "Globals.hpp"
#include <Misc/BuildInfo.hpp>

std::string Anvil::g_BuildInfo = "Anvil Client - " + std::string(__DATE__) + " - Build: " + std::to_string(ANVIL_BUILD);
void* Anvil::g_ThreadStorage = Anvil::Internal_GetThreadStorage();

void* Anvil::Internal_GetThreadStorage()
{
	void* s_ThreadStorage = nullptr;
	__asm
	{
		mov	eax,	dword ptr fs:[2Ch]
		mov	eax,	dword ptr ds:[eax]
		mov	s_ThreadStorage,	eax
	}

	return s_ThreadStorage;
}

template<typename T>
T* Anvil::GetThreadStorage(size_t p_Offset)
{
	if (!g_ThreadStorage)
		return nullptr;

	auto s_Storage = *reinterpret_cast<T**>(g_ThreadStorage + p_Offset);

	return s_Storage;
}