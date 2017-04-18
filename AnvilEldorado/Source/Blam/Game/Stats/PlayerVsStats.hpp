#pragma once
#include <cstdint>

namespace Blam::Game::Stats
{
	struct PlayerVsStats
	{
		uint16_t Kills;
		uint16_t KilledBy;
	};
}