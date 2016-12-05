#include "Globals.hpp"
#include "Players.hpp"

namespace Blam::Game
{
	Data::DataArray<PlayerDatum> &GetPlayers()
	{
		return *AnvilCommon::GetThreadStorage<Data::DataArray<PlayerDatum>>(0x40);
	}

	Data::DatumIndex GetLocalPlayer(const int32_t p_Index)
	{
		typedef uint32_t(*GetLocalPlayerPtr)(int index);
		auto GetLocalPlayer = reinterpret_cast<GetLocalPlayerPtr>(0x589C30);
		return GetLocalPlayer(p_Index);
	}
}