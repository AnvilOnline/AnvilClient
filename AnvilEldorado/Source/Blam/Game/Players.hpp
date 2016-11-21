#pragma once
#include <cstdint>
#include "..\Data\DatumBase.hpp"
#include "..\Data\DatumIndex.hpp"
#include "..\Data\DataArray.hpp"

namespace Blam::Game
{
	enum class PlayerColor
	{
		Primary,
		Secondary,
		Visor,
		Lights,
		Holo,

		Count
	};

	enum class PlayerArmor
	{
		Helmet = 0,
		Chest,
		Shoulders,
		Arms,
		Legs,
		Acc,
		Pelvis,

		Count
	};

	struct PlayerCustomization
	{
		uint32_t Colors[(int)PlayerColor::Count];
		uint8_t Armor[(int)PlayerArmor::Count];
		uint32_t Unknown1C;
	};
	static_assert(sizeof(PlayerCustomization) == 0x20, "Blam::Game::PlayerCustomization");

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

	struct PlayerDatum : Data::DatumBase
	{
		uint8_t Unknown2[0x2E];
		Data::DatumIndex SlaveUnit;
		uint8_t Unknown34[0x3C];
		PlayerProperties Properties;
		uint8_t Unknown1690[0x1724];
		int16_t SpawnGrenadeSetting;
		uint8_t Unknown2DB6[0x152];
	};
	static_assert(sizeof(PlayerDatum) == 0x2F08, "Blam::Game::PlayerDatum");

	Data::DataArray<PlayerDatum> &GetPlayers();

	Data::DatumIndex GetLocalPlayer(const size_t p_Index);
}