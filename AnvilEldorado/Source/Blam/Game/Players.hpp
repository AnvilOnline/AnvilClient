#pragma once
#include <cstdint>
#include <vector>
#include <memory>

#include "Blam\Data\BitStream.hpp"
#include "Blam\Data\DatumBase.hpp"
#include "Blam\Data\DatumIndex.hpp"
#include "Blam\Data\DataArray.hpp"

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

	Data::DatumIndex GetLocalPlayer(const int32_t p_Index);

	// Base class for a class which adds data to player properties.
	class PlayerPropertiesExtensionBase
	{
	public:
		virtual ~PlayerPropertiesExtensionBase() { }

		// Builds extension data for a player.
		virtual void BuildData(int32_t playerIndex, void *out) = 0;

		// Gets the size of the extension data.
		virtual size_t GetDataSize() const = 0;

		// Applies extension data to a player.
		virtual void ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const void *data) = 0;

		// Serializes the extension data to be sent across the network.
		virtual void Serialize(Blam::Data::BitStream *stream, const void *data) = 0;

		// Deserializes extension data that was received from the network.
		virtual void Deserialize(Blam::Data::BitStream *stream, void *out) = 0;
	};

	// Helper class which adds type safety to PlayerPropertiesExtensionBase.
	template <class TData>
	class PlayerPropertiesExtension : public PlayerPropertiesExtensionBase
	{
	protected:
		// Builds extension data for a player.
		virtual void BuildData(int32_t playerIndex, TData *out) = 0;

		// Applies extension data to a player.
		virtual void ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const TData &data) = 0;

		// Serializes the extension data to be sent across the network.
		virtual void Serialize(Blam::Data::BitStream *stream, const TData &data) = 0;

		// Deserializes extension data that was received from the network.
		virtual void Deserialize(Blam::Data::BitStream *stream, TData *out) = 0;

	public:

		void BuildData(int32_t playerIndex, void *out) override
		{
			BuildData(playerIndex, static_cast<TData*>(out));
		}

		size_t GetDataSize() const override
		{
			return sizeof(TData);
		}

		void ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const void *data) override
		{
			ApplyData(playerIndex, properties, *static_cast<const TData*>(data));
		}

		void Serialize(Blam::Data::BitStream *stream, const void *data) override
		{
			Serialize(stream, *static_cast<const TData *>(data));
		}

		void Deserialize(Blam::Data::BitStream *stream, void *out) override
		{
			Deserialize(stream, static_cast<TData *>(out));
		}
	};

	// Singleton object which lets the player-properties packet be extended with custom data
	// TODO: Make this more generic and not so specific to player-properties
	class PlayerPropertiesExtender
	{
	public:
		// Adds an extension to the player-properties packet.
		void Add(std::shared_ptr<PlayerPropertiesExtensionBase> extension);

		// Gets the total size of the player-properties extension data.
		size_t GetTotalSize();

		// Writes all extension data out to a player-properties structure.
		void BuildData(int32_t playerIndex, void *out);

		// Applies all extension data in a player-properties structure.
		void ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const void *data);

		// Serializes all extension data in a player-properties structure.
		void SerializeData(Blam::Data::BitStream *stream, const void *data);

		// Deserializes all extension data in a player-properties structure.
		void DeserializeData(Blam::Data::BitStream *stream, void *out);

	private:
		std::vector<std::shared_ptr<PlayerPropertiesExtensionBase>> extensions;
	};

	// Packet size constants
	extern const size_t PlayerPropertiesPacketHeaderSize;
	extern const size_t PlayerPropertiesSize;
	extern const size_t PlayerPropertiesPacketFooterSize;

	size_t GetPlayerPropertiesPacketSize();
}