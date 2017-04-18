#pragma once
#include "PlayerPropertiesExtensionBase.hpp"
#include <memory>
#include <vector>

namespace AnvilEldorado::Game::Players
{
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
		void ApplyData(int32_t playerIndex, Blam::Game::Players::PlayerProperties *properties, const void *data);

		// Serializes all extension data in a player-properties structure.
		void SerializeData(Blam::Data::BitStream *stream, const void *data);

		// Deserializes all extension data in a player-properties structure.
		void DeserializeData(Blam::Data::BitStream *stream, void *out);

	private:
		std::vector<std::shared_ptr<PlayerPropertiesExtensionBase>> extensions;
	};
}