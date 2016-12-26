#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Utils\Logger.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"

#include "Blam\Cache\StringIDCache.hpp"

#include "Globals.hpp"
#include "Graphics.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "Network.hpp"
#include "Camera.hpp"
#include "UserInterface.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Forge.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
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

	__declspec(naked) void OnTagsLoaded_Hook()
	{
		__asm
		{
			call TagsLoadedImpl
			push 0x6D617467
			push 0x5030EF
			ret
		}
	}
	
	const auto Game_InitAudioSystem = reinterpret_cast<bool(*)()>(0x64E190);

	void *Game_InitHalo3_Hook()
	{
		Game_InitAudioSystem();

		return nullptr;
	}

	char Video_GetBinkVideoPath_Hook(int p_VideoID, char *p_DestBuf)
	{
		// Tell the game that there is no video with that ID
		return 0;
	}

	bool Engine::Init()
	{
		return ApplyPatches()
			&& Graphics::Instance()->Init()
			&& Audio::Instance()->Init()
			&& Input::Instance()->Init()
			&& Network::Instance()->Init()
			&& Camera::Instance()->Init()
			&& UserInterface::Instance()->Init()
			&& Game::Instance()->Init()
			&& Player::Instance()->Init()
			&& Forge::Instance()->Init();
	}

	bool Engine::UnprotectMemory()
	{
		size_t s_Offset = 0, s_Total = 0;

		MEMORY_BASIC_INFORMATION s_MemInfo;

		while (VirtualQuery((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset, &s_MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
		{
			s_Offset += s_MemInfo.RegionSize;

			if (s_MemInfo.Protect == PAGE_EXECUTE_READ)
			{
				s_Total += s_MemInfo.RegionSize;
				return VirtualProtect(s_MemInfo.BaseAddress, s_MemInfo.RegionSize, PAGE_EXECUTE_READWRITE, &s_MemInfo.Protect) == TRUE;
			}
		}

		return false;
	}

	bool Engine::ApplyPatches()
	{
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			//Disable Windows DPI scaling
		return SetProcessDPIAware()
			// Enable write to all executable memory
			&& UnprotectMemory()
			// No --account args
			&& Patch(0x43731A, { 0xEB, 0x0E }).Apply()
			&& Patch(0x4373AD, { 0xEB, 0x03 }).Apply()
			// Disable saber's additions to the engine
			&& Hook(0x200990, Game_InitHalo3_Hook).Apply()
			// Disable Bink videos
			&& Hook(0x699120, Video_GetBinkVideoPath_Hook).Apply()
			// Fix random colored lighting
			&& Patch(0x14F2FFC, { 0x0, 0x0, 0x0, 0x0 }).Apply()
			// Enable tag edits
			&& Patch(0x101A5B, 0xEB).Apply()
			&& Patch::NopFill(0x102874, 2)
			&& Patch::NopFill(0x1030AA, 2)
			&& Hook(0x1030EA, OnTagsLoaded_Hook).Apply();
	}

	bool Engine::OnTagsLoaded()
	{
		UserInterface::Instance()->ApplyResolution();

		return Player::Instance()->OnTagsLoaded();
	}

	bool Engine::HasMainMenuShown() const
	{
		return m_MainMenuShown;
	}

	void Engine::SetMainMenuShown(const bool p_Shown)
	{
		m_MainMenuShown = p_Shown;
	}
}