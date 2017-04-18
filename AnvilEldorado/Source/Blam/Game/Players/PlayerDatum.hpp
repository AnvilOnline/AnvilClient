#pragma once
#include <Blam\Data\DatumBase.hpp>
#include <Blam\Data\DatumIndex.hpp>
#include "PlayerProperties.hpp"

namespace Blam::Game::Players
{
	struct PlayerDatum : Data::DatumBase
	{
		uint8_t Unknown2[0x2E];
		Data::DatumIndex SlaveUnit;
		uint8_t Unknown34[0x3C];
		PlayerProperties Properties;
		uint8_t Unknown1690[0x1724];
		int16_t SpawnGrenadeSetting;
		uint8_t Unknown2DB6[0x152];
	};
	static_assert(sizeof(PlayerDatum) == 0x2F08, "Blam::Game::PlayerDatum");
}