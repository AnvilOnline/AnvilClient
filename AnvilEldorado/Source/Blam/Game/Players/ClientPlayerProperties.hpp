#pragma once
#include <cstdint>

namespace Blam::Game::Players
{
	struct ClientPlayerProperties
	{
		wchar_t DisplayName[16];
		int8_t TeamIndex;
		int16_t Unknown22;
		uint32_t Unknown24;
		uint32_t Unknown28;
		uint32_t Unknown2C;
	};
	static_assert(sizeof(ClientPlayerProperties) == 0x30, "Blam::Game::ClientPlayerProperties");
}