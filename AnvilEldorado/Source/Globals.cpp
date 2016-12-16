#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <Globals.hpp>
#include <BuildInfo.hpp>

std::string AnvilCommon::g_BuildInfo = "AnvilEldorado Client - " + std::string(__DATE__) + " - Build: " + std::to_string(ANVIL_BUILD);

void *AnvilCommon::Internal_GetThreadStorage(const size_t p_Offset)
{
	static void *s_ThreadStorage = nullptr;

	if (!s_ThreadStorage)
	{
		_asm
		{
			mov     eax, dword ptr fs : [2Ch]
			mov     eax, dword ptr ds : [eax]
			mov		s_ThreadStorage, eax
		}
	}

	return (uint8_t *)s_ThreadStorage + p_Offset;
}

void *AnvilCommon::Internal_GetModuleStorage(const size_t p_Offset)
{
	static void *s_ModuleStorage = nullptr;

	if (s_ModuleStorage == nullptr)
	{
		auto s_Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

		if (s_Snapshot == INVALID_HANDLE_VALUE)
			return nullptr;

		MODULEENTRY32 s_ModuleEntry;
		s_ModuleEntry.dwSize = sizeof(MODULEENTRY32);

		auto s_Module = Module32First(s_Snapshot, &s_ModuleEntry);

		CloseHandle(s_Snapshot);

		s_ModuleStorage = (s_Module != 0) ? (void*)s_ModuleEntry.modBaseAddr : nullptr;
	}

	return s_ModuleStorage;
}