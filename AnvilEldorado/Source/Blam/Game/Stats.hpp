#pragma once
#include "Stats\PlayerStats.hpp"
#include "Stats\PlayerKilledPlayerStats.hpp"

#include <Blam\Data\DatumIndex.hpp>

namespace Blam::Game
{
	int32_t GetKills(const Data::DatumIndex &p_Player);

	int32_t GetDeaths(const Data::DatumIndex &p_Player);

	int32_t GetAssists(const Data::DatumIndex &p_Player);

	Stats::PlayerStats GetStats(const size_t p_PlayerIndex);

	Stats::PlayerKilledPlayerStats GetPVPStats(const size_t p_PlayerIndex);
}