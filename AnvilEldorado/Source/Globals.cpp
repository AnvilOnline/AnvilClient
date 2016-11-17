#include <Globals.hpp>
#include <BuildInfo.hpp>

std::string AnvilCommon::g_BuildInfo = "AnvilEldorado Client - " + std::string(__DATE__) + " - Build: " + std::to_string(ANVIL_BUILD);
void* AnvilCommon::g_ThreadStorage = AnvilCommon::Internal_GetThreadStorage();

void* AnvilCommon::Internal_GetThreadStorage()
{
	void* s_ThreadStorage = nullptr;
	__asm
	{
		mov	eax, dword ptr fs : [2Ch]
		mov	eax, dword ptr ds : [eax]
		mov	s_ThreadStorage, eax
	}

	return s_ThreadStorage;
}