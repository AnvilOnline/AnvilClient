#pragma once
#include "PlayerVsStats.hpp"

namespace Blam::Game::Stats
{
	struct PlayerKilledPlayerStats
	{
		PlayerVsStats StatsAgainstEachPlayer[16];
	};
}