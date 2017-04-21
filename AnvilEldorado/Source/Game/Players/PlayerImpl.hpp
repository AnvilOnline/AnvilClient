#pragma once
#include <Interfaces\Initializable.hpp>

#include <Blam\Data\DataArray.hpp>
#include <Blam\Data\DatumIndex.hpp>
#include <Blam\Game\Players\PlayerDatum.hpp>
#include <Game\Players\PlayerPropertiesExtender.hpp>

namespace AnvilEldorado::Game::Players
{
	class PlayerImpl :
		public AnvilCommon::Initializable
	{

	private:
		PlayerPropertiesExtender* playerPropertiesExtender = new PlayerPropertiesExtender();;
	public:
		virtual bool Init();

		// TODO: Fix
		Blam::Data::DataArray<Blam::Game::Players::PlayerDatum> &GetPlayers();
		Blam::Data::DatumIndex GetLocalPlayer(const int32_t p_Index);
		size_t GetPlayerPropertiesPacketSize();
	};

	// Packet size constants
	extern const size_t PlayerPropertiesPacketHeaderSize;
	extern const size_t PlayerPropertiesSize;
	extern const size_t PlayerPropertiesPacketFooterSize;
}
