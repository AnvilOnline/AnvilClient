#pragma once
#include <Blam\Data\BitStream.hpp>
#include <Blam\Game\Players\PlayerProperties.hpp>

namespace AnvilEldorado::Game::Players
{
	// Base class for a class which adds data to player properties.
	class PlayerPropertiesExtensionBase
	{
	public:
		virtual ~PlayerPropertiesExtensionBase() {}

		// Builds extension data for a player.
		virtual void BuildData(int32_t playerIndex, void *out) = 0;

		// Gets the size of the extension data.
		virtual size_t GetDataSize() const = 0;

		// Applies extension data to a player.
		virtual void ApplyData(int32_t playerIndex, Blam::Game::Players::PlayerProperties *properties, const void *data) = 0;

		// Serializes the extension data to be sent across the network.
		virtual void Serialize(Blam::Data::BitStream *stream, const void *data) = 0;

		// Deserializes extension data that was received from the network.
		virtual void Deserialize(Blam::Data::BitStream *stream, void *out) = 0;
	};
}