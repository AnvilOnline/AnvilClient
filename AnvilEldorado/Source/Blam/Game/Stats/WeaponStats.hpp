#pragma once
#include <cstdint>

namespace Blam::Game::Stats
{
	struct WeaponStats
	{
		uint16_t Initialized;
		uint16_t Kills;
		uint16_t KilledBy;
		uint16_t BetrayalsWith;
		uint16_t SuicidesWith;
		uint16_t HeadshotsWith;
	};
}