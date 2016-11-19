#pragma once
#include <cstdint>
#include "..\Data\DatumIndex.hpp"
#include "Medals.hpp"
#include "Damage.hpp"

namespace Blam::Game
{
	struct PlayerVsStats
	{
		uint16_t Kills;
		uint16_t KilledBy;
	};

	struct PlayerKilledPlayerStats
	{
		PlayerVsStats StatsAgainstEachPlayer[16];
	};

	struct WeaponStats
	{
		uint16_t Initialized;
		uint16_t Kills;
		uint16_t KilledBy;
		uint16_t BetrayalsWith;
		uint16_t SuicidesWith;
		uint16_t HeadshotsWith;
	};

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

	int32_t GetKills(const Data::DatumIndex &p_Player);

	int32_t GetDeaths(const Data::DatumIndex &p_Player);

	int32_t GetAssists(const Data::DatumIndex &p_Player);

	PlayerStats GetStats(const size_t p_PlayerIndex);

	PlayerKilledPlayerStats GetPVPStats(const size_t p_PlayerIndex);
}