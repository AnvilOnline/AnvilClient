#include "Globals.hpp"
#include "Players.hpp"

#include <Blam\Game\Players\PlayerDatum.hpp>

namespace Blam::Game
{
	Data::DataArray<Players::PlayerDatum> &GetPlayers()
	{
		return *AnvilCommon::GetThreadStorage<Data::DataArray<Players::PlayerDatum>>(0x40);
	}

	Data::DatumIndex GetLocalPlayer(const int32_t p_Index)
	{
		typedef uint32_t(*GetLocalPlayerPtr)(int index);
		auto GetLocalPlayer = reinterpret_cast<GetLocalPlayerPtr>(0x589C30);
		return GetLocalPlayer(p_Index);
	}



	const size_t PlayerPropertiesPacketHeaderSize = 0x18;
	const size_t PlayerPropertiesSize = 0x30;
	const size_t PlayerPropertiesPacketFooterSize = 0x4;

	size_t GetPlayerPropertiesPacketSize()
	{
		static size_t size;

		if (size == 0)
		{
			// TODO: Fix
			size_t extensionSize = 0; // PlayerPropertiesExtender::Instance()->GetTotalSize();
			size = PlayerPropertiesPacketHeaderSize + PlayerPropertiesSize + extensionSize + PlayerPropertiesPacketFooterSize;
		}

		return size;
	}
}