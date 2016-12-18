#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Globals.hpp"
#include "Utils\Logger.hpp"
#include "Utils\Util.hpp"
#include "Blam\Cache\StringIDCache.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	void UnprotectMemory()
	{
		size_t s_Offset = 0, s_Total = 0;

		MEMORY_BASIC_INFORMATION s_MemInfo;

		while (VirtualQuery((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset, &s_MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
		{
			s_Offset += s_MemInfo.RegionSize;

			if (s_MemInfo.Protect == PAGE_EXECUTE_READ)
			{
				s_Total += s_MemInfo.RegionSize;
				VirtualProtect(s_MemInfo.BaseAddress, s_MemInfo.RegionSize, PAGE_EXECUTE_READWRITE, &s_MemInfo.Protect);
			}
		}
	}

	void TagsLoadedImpl()
	{
		WriteLog("Tags loaded successfully.");

		if (!Blam::Cache::StringIDCache::Instance()->Load("maps\\string_ids.dat"))
		{
			WriteLog("Failed to load 'maps\\string_ids.dat'!");
			// TODO: Exit game?
			return;
		}

		if (!Engine::Instance()->OnTagsLoaded())
		{
			WriteLog("ERROR: Failed to apply changes after tags were loaded!");
			// TODO: Exit game?
		}
	}

	__declspec(naked) void TagsLoadedHook()
	{
		__asm
		{
			call TagsLoadedImpl
			push 0x6D617467
			push 0x5030EF
			ret
		}
	}
	
	void *Game_InitSaberCode()
	{
		//Fix the audio being disabled
		typedef char(*sub_64E190Func)();
		sub_64E190Func sub_64E190 = (sub_64E190Func)0x64E190;
		sub_64E190();

		return nullptr;
	}

	char Game_LoadBinkVideo()
	{
		return 1;
	}

	bool Engine::ApplyPatches_Core()
	{
		using AnvilCommon::Utils::Util;

		//Disable Windows DPI scaling
		SetProcessDPIAware();

		// Enable write to all executable memory
		UnprotectMemory();

			// No --account args
		return Util::PatchAddress(0x43731A, "\xEB\x0E", 2)
			&& Util::PatchAddress(0x4373AD, "\xEB\x03", 2)
			// Disable saber's additions to the engine
			&& Util::ApplyHook(0x200990, Game_InitSaberCode)
			// Disable BIK videos
			&& Util::ApplyHook(0x699120, Game_LoadBinkVideo)
			// Set game locale to english
			&& Util::PatchAddress(0x2333FD, "\x00", 1)
			// Enable tag edits
			&& Util::PatchAddress(0x101A5B, "\xEB", 1)
			&& Util::PatchAddress(0x102874, "\x90\x90", 2)
			&& Util::PatchAddress(0x1030AA, "\x90\x90", 2)
			&& Util::ApplyHook(0x1030EA, TagsLoadedHook);
	}
}