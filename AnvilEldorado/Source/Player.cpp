#include "Utils\Hook.hpp"
#include "Engine.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Tags\Scenario\Scenario.hpp"

namespace AnvilEldorado
{
	void GrenadeLoadoutHookImpl(uint8_t *p_UnitData)
	{
		// Based off of 0x8227B48C in H3 non-TU

		// TODO: Clean this up, hardcoded offsets are hacky
		const size_t s_GrenadeCountOffset = 0x320;
		const size_t s_ControllingPlayerOffset = 0x198;
		auto *s_GrenadeCounts = p_UnitData + s_GrenadeCountOffset; // 0 = frag, 1 = plasma, 2 = spike, 3 = firebomb
		auto s_PlayerIndex = *reinterpret_cast<int16_t *>(p_UnitData + s_ControllingPlayerOffset);
		if (s_PlayerIndex < 0)
		{
			memset(s_GrenadeCounts, 0, 4);
			return;
		}

		// Get the player's grenade setting
		auto &s_Players = Blam::Game::GetPlayers();
		auto s_GrenadeSetting = s_Players[s_PlayerIndex].SpawnGrenadeSetting;

		// Get the current scenario tag
		auto *s_Scenario = Blam::Tags::Scenario::GetCurrentScenario();

		// If the setting is none (2) or the scenario has invalid starting
		// profile data, set the grenade counts to 0 and return
		if (s_GrenadeSetting == 2 || !s_Scenario->StartingProfile)
		{
			memset(s_GrenadeCounts, 0, 4);
			return;
		}

		// Load the grenade counts from the scenario tag
		auto *s_Profile = &s_Scenario->StartingProfile[0];
		s_GrenadeCounts[0] = s_Profile->FragGrenades;
		s_GrenadeCounts[1] = s_Profile->PlasmaGrenades;
		s_GrenadeCounts[2] = s_Profile->SpikeGrenades;
		s_GrenadeCounts[3] = s_Profile->FirebombGrenades;
	}

	__declspec(naked) void GrenadeLoadoutHook()
	{
		__asm
		{
			push edi // Unit object data
			call GrenadeLoadoutHookImpl
			add esp, 4
			push 0x5A32C7
			ret
		}
	}

	bool Engine::ApplyPatches_Player()
	{
		using AnvilCommon::Utils::Hook;

			// Fix grenade loadouts
		return Hook(0x1A3267, GrenadeLoadoutHook).Apply();
	}
}