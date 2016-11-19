#include "Utils\Util.hpp"
#include "Player.hpp"

namespace Blam::Game
{
	Data::DataArray<PlayerDatum> &GetPlayers()
	{
		return *reinterpret_cast<Data::DataArray<PlayerDatum> *>(AnvilCommon::Utils::Util::GetTls(0x40));
	}

	Data::DatumIndex GetLocalPlayer(const size_t p_Index)
	{
		return reinterpret_cast<uint32_t(*)(int)>(0x589C30)(p_Index);
	}
}