#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Globals.hpp"
#include "Utils\Logger.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
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

	char Video_GetBinkVideoPath(int videoID, char *dstBuf)
	{
		// Tell the game that there is no video with that ID
		return 0;
	}

	bool Engine::ApplyPatches_Core()
	{
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

		//Disable Windows DPI scaling
		SetProcessDPIAware();

		// Enable write to all executable memory
		UnprotectMemory();

			// No --account args
		return Patch(0x43731A, { 0xEB, 0x0E }).Apply()
			&& Patch(0x4373AD, { 0xEB, 0x03 }).Apply()
			// Disable saber's additions to the engine
			&& Hook(0x200990, Game_InitSaberCode).Apply()
			// Disable Bink videos
			&& Hook(0x699120, Video_GetBinkVideoPath).Apply()
			// Fix random colored lighting
			&& Patch(0x14F2FFC, { 0x0, 0x0, 0x0, 0x0 }).Apply()
			// Enable tag edits
			&& Patch(0x101A5B, 0xEB).Apply()
			&& Patch::NopFill(0x102874, 2)
			&& Patch::NopFill(0x1030AA, 2)
			&& Hook(0x1030EA, TagsLoadedHook).Apply();
	}
}