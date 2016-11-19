#include "Stats.hpp"

namespace Blam::Game
{
	int32_t GetKills(const Data::DatumIndex &p_Player)
	{
		return reinterpret_cast<int(*)(Data::DatumIndex)>(0x550620)(p_Player);
	}

	int32_t GetDeaths(const Data::DatumIndex &p_Player)
	{
		return reinterpret_cast<int(*)(Data::DatumIndex)>(0x550580)(p_Player);
	}

	int32_t GetAssists(const Data::DatumIndex &p_Player)
	{
		return reinterpret_cast<int(*)(Data::DatumIndex)>(0x5504E0)(p_Player);
	}

	PlayerStats GetStats(const size_t p_PlayerIndex)
	{
		return *reinterpret_cast<PlayerStats *>(0x23F1718 + (p_PlayerIndex * sizeof(PlayerStats)));
	}

	PlayerKilledPlayerStats GetPVPStats(const size_t p_PlayerIndex)
	{
		return *reinterpret_cast<PlayerKilledPlayerStats *>(0x23F5A98 + (p_PlayerIndex * sizeof(PlayerKilledPlayerStats)));
	}
}