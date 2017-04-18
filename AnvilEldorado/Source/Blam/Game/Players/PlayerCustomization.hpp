#pragma once
#include <cstdint>

#include "PlayerColor.hpp"
#include "PlayerArmor.hpp"

namespace Blam::Game::Players
{
	struct PlayerCustomization
	{
		uint32_t Colors[(int)PlayerColor::Count];
		uint8_t Armor[(int)PlayerArmor::Count];
		uint32_t Unknown1C;
	};
	static_assert(sizeof(PlayerCustomization) == 0x20, "Blam::Game::PlayerCustomization");
}