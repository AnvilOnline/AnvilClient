#pragma once
#include "WeaponStats.hpp"

#include <Blam\Game\Damage.hpp>
#include <Blam\Game\Medals.hpp>

#include <cstdint>


namespace Blam::Game::Stats
{
	struct PlayerStats
	{
		uint8_t Unknown0[0xC];
		int16_t Score;
		uint16_t Unknown1;
		int16_t Kills;
		uint16_t Assists;
		uint16_t Deaths;
		uint16_t Betrayals;
		uint16_t Suicides;
		uint16_t BestStreak;
		uint16_t TimeSpentAlive;
		uint8_t Unknown3[0x1A];
		uint16_t KingsKilled;
		uint16_t TimeInHill;
		uint16_t TimeControllingHill;
		uint8_t Unknown4[0x22];
		uint16_t ZombiesKilled;
		uint16_t HumansInfected;
		uint16_t Unknown5;
		uint16_t Medals[(int)MedalType::Count];
		uint8_t Unknown6[0x68];
		WeaponStats WeaponStats[(int)DamageReportingType::Count];
	};
	static_assert(sizeof(PlayerStats) == 0x438, "Blam::Game::PlayerStats");
}