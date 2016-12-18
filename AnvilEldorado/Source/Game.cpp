#include "Globals.hpp"
#include "Utils\Util.hpp"
#include "Blam\Network\Session.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	void ApplyMapNameFixes()
	{
		auto s_MapNamesAddress = *reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x149E2E0);
		if (!s_MapNamesAddress)
			return;

		// TODO: map out these global arrays, content items seems to use same format

		uint32_t s_MapNameCount = *reinterpret_cast<uint32_t *>(s_MapNamesAddress + 0x34);

		const wchar_t* s_Search[12] = { L"guardian", L"riverworld", L"s3d_avalanche", L"s3d_edge", L"s3d_reactor", L"s3d_turf", L"cyberdyne", L"chill", L"deadlock", L"bunkerworld", L"shrine", L"zanzibar" };
		const wchar_t* s_Names[12] = { L"Guardian", L"Valhalla", L"Diamondback", L"Edge", L"Reactor", L"Icebox", L"The Pit", L"Narrows", L"High Ground", L"Standoff", L"Sandtrap", L"Last Resort" };
		// TODO: Get names/descs using string ids? Seems the unic tags have descs for most of the maps
		const wchar_t* s_Descriptions[12] = {
			L"Millennia of tending has produced trees as ancient as the Forerunner structures they have grown around. 2-6 players.",
			L"The crew of V-398 barely survived their unplanned landing in this gorge...this curious gorge. 6-16 players.",
			L"Hot winds blow over what should be a dead moon. A reminder of the power Forerunners once wielded. 6-16 players.",
			L"The remote frontier world of Partition has provided this ancient databank with the safety of seclusion. 6-16 players.",
			L"Being constructed just prior to the Invasion, its builders had to evacuate before it was completed. 6-16 players.",
			L"Downtown Tyumen's Precinct 13 offers an ideal context for urban combat training. 4-10 players.",
			L"Software simulations are held in contempt by the veteran instructors who run these training facilities. 4-10 players.",
			L"Without cooling systems such as these, excess heat from the Ark's forges would render the construct uninhabitable. 2-8 players.",
			L"A relic of older conflicts, this base was reactivated after the New Mombasa Slipspace Event. 4-12 players.",
			L"Once, nearby telescopes listened for a message from the stars. Now, these silos contain our prepared response. 4-12 players.",
			L"Although the Brute occupiers have been driven from this ancient structure, they left plenty to remember them by. 6-16 players",
			L"Remote industrial sites like this one are routinely requisitioned and used as part of Spartan training exercises. 4-12 players."

		};

		for (uint32_t i = 0; i < s_MapNameCount; i++)
		{
			auto *s_MapNameAddress = reinterpret_cast<wchar_t *>(s_MapNamesAddress + 0x54 + (0x360 * i) + 0x8);
			auto *s_MapDescriptionAddress = reinterpret_cast<wchar_t *>(s_MapNamesAddress + 0x54 + (0x360 * i) + 0x8 + 0x40);

			wchar_t s_MapName[0x21] = { 0 };
			memcpy(s_MapName, s_MapNameAddress, 0x20);

			for (uint32_t y = 0; y < sizeof(s_Search) / sizeof(*s_Search); y++)
			{
				if (wcscmp(s_Search[y], s_MapName) == 0)
				{
					memset(s_MapNameAddress, 0, sizeof(wchar_t) * 0x20);
					wcscpy_s(s_MapNameAddress, 0x20, s_Names[y]);

					memset(s_MapDescriptionAddress, 0, sizeof(wchar_t) * 0x80);
					wcscpy_s(s_MapDescriptionAddress, 0x80, s_Descriptions[y]);

					break;
				}
			}
		}
	}

	void GameTickHook()
	{
		static bool s_AppliedFirstTickPatches = false;

		if (!s_AppliedFirstTickPatches)
		{
			ApplyMapNameFixes();
			s_AppliedFirstTickPatches = true;
		}

		// Call replaced function
		reinterpret_cast<void(*)()>(0x5547F0)();
	}

	void ShutdownHook()
	{
		//
		// TODO: Call any shutdown functions...
		//

		// Call the replaced function
		reinterpret_cast<void(*)()>(0x42E410)();
	}

	typedef std::function<void(const char *p_MapPath)> MapLoadedCallback;
	std::vector<MapLoadedCallback> g_MapLoadedCallbacks;

	const auto LoadMap = reinterpret_cast<bool(*)(void *p_Data)>(0x566EF0);

	bool LoadMapHook(void *p_Data)
	{
		if (!LoadMap(p_Data))
			return false;

		for (auto &s_Callback : g_MapLoadedCallbacks)
			s_Callback(static_cast<const char *>(p_Data) + 0x24); // hax

		return true;
	}

	void EndGameHook()
	{
		auto *session = Blam::Network::GetActiveSession();

		if (!session || !session->IsEstablished())
			return;

		if (session->IsHost())
			Blam::Network::EndGame();
		else
			Blam::Network::LeaveGame();
	}

	bool Engine::ApplyPatches_Game()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Util;

		auto *s_ModuleBase = AnvilCommon::Internal_GetModuleStorage();

		// Update countdown timer
		*reinterpret_cast<uint8_t *>((uint8_t *)s_ModuleBase + 0x153708) = 5; // player control
		*reinterpret_cast<uint8_t *>((uint8_t *)s_ModuleBase + 0x153738) = 9; // camera position
		*reinterpret_cast<uint8_t *>((uint8_t *)s_ModuleBase + 0x1521D1) = 9; // ui timer
		*reinterpret_cast<uint8_t *>((uint8_t *)s_ModuleBase + 0x1536F0) = 3; // team notification

			// Hook game ticks
		return Util::ApplyHook(0x105ABA, GameTickHook, HookFlags::IsCall)
			&& Util::ApplyHook(0x1063E6, GameTickHook, HookFlags::IsCall)
			// Run callbacks on engine shutdown
			&& Util::ApplyHook(0x2EBD7, ShutdownHook, HookFlags::IsCall)
			// Map loading
			&& Util::ApplyHook(0x10FC2C, LoadMapHook, HookFlags::IsCall)
			&& Util::ApplyHook(0x1671BE, LoadMapHook, HookFlags::IsCall)
			&& Util::ApplyHook(0x167B4F, LoadMapHook, HookFlags::IsCall)
			// Rewire $hq.MatchmakingLeaveQueue() to end the game
			&& Util::ApplyHook(0x3B6826, EndGameHook, HookFlags::IsCall)
			&& Util::PatchAddress(0x3B682B, "\x90", 1)
			// Prevent game variant weapons from being overridden
			&& Util::PatchAddress(0x1A315F, "\xEB", 1)
			&& Util::PatchAddress(0x1A31A4, "\xEB", 1);
	}
}