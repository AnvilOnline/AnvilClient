#pragma once
#include <cstdint>
#include <vector>
#include <memory>

#include <Blam\Data\DataArray.hpp>
#include <Blam\Game\Players\PlayerDatum.hpp>

namespace Blam::Game
{

	// TODO: Fix
	Data::DataArray<Players::PlayerDatum> &GetPlayers();

	Data::DatumIndex GetLocalPlayer(const int32_t p_Index);


	// Packet size constants
	extern const size_t PlayerPropertiesPacketHeaderSize;
	extern const size_t PlayerPropertiesSize;
	extern const size_t PlayerPropertiesPacketFooterSize;

	size_t GetPlayerPropertiesPacketSize();
}