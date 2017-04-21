#include "PlayerImpl.hpp"

#include <Game\Players\PlayerArmorExtender.hpp>

#include <Globals.hpp>

using namespace AnvilEldorado::Game::Players;

const size_t AnvilEldorado::Game::Players::PlayerPropertiesPacketHeaderSize = 0x18;
const size_t AnvilEldorado::Game::Players::PlayerPropertiesSize = 0x30;
const size_t AnvilEldorado::Game::Players::PlayerPropertiesPacketFooterSize = 0x4;

bool PlayerImpl::Init()
{
	playerPropertiesExtender->Add(std::make_shared<PlayerArmorExtension>());
	// TODO: Put any init logic here
	return true;
}


Blam::Data::DataArray<Blam::Game::Players::PlayerDatum> &PlayerImpl::GetPlayers()
{
	return *AnvilCommon::GetThreadStorage<Blam::Data::DataArray<Blam::Game::Players::PlayerDatum>>(0x40);
}

Blam::Data::DatumIndex PlayerImpl::GetLocalPlayer(const int32_t p_Index)
{
	typedef uint32_t(*GetLocalPlayerPtr)(int index);
	auto GetLocalPlayer = reinterpret_cast<GetLocalPlayerPtr>(0x589C30);
	return GetLocalPlayer(p_Index);
}

size_t PlayerImpl::GetPlayerPropertiesPacketSize()
{
	static size_t size;

	if (size == 0)
	{
		size_t extensionSize = playerPropertiesExtender->GetTotalSize();
		size = PlayerPropertiesPacketHeaderSize + PlayerPropertiesSize + extensionSize + PlayerPropertiesPacketFooterSize;
	}

	return size;
}