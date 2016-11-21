#include "Globals.hpp"
#include "Players.hpp"

namespace Blam::Game
{
	Data::DataArray<PlayerDatum> &GetPlayers()
	{
		return *AnvilCommon::GetThreadStorage<Data::DataArray<PlayerDatum>>(0x40);
	}

	Data::DatumIndex GetLocalPlayer(const size_t p_Index)
	{
		return reinterpret_cast<uint32_t(*)(int)>(0x589C30)(p_Index);
	}
}