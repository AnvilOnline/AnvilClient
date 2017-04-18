#pragma once
#include <cstdint>
#include "ClientPlayerProperties.hpp"
#include "PlayerCustomization.hpp"

namespace Blam::Game::Players
{
	struct PlayerProperties
	{
		ClientPlayerProperties ClientProperties;
		uint64_t Uid;
		wchar_t DisplayName[16];
		int TeamIndex;
		uint8_t Unknown5C[0x66C];
		PlayerCustomization Customization;
		uint8_t Unknown6E4[0xF38];
	};
	static_assert(sizeof(PlayerProperties) == 0x1620, "Blam::Game::PlayerProperties");
}