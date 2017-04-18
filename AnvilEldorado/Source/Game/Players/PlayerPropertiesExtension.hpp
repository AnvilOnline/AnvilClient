#pragma once
#include "PlayerPropertiesExtensionBase.hpp"

namespace AnvilEldorado::Game::Players
{
	// Helper class which adds type safety to PlayerPropertiesExtensionBase.
	template <class TData>
	class PlayerPropertiesExtension : public PlayerPropertiesExtensionBase
	{
	protected:
		// Builds extension data for a player.
		virtual void BuildData(int32_t playerIndex, TData *out) = 0;

		// Applies extension data to a player.
		virtual void ApplyData(int32_t playerIndex, Blam::Game::Players::PlayerProperties *properties, const TData &data) = 0;

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

		void ApplyData(int32_t playerIndex, Blam::Game::Players::PlayerProperties *properties, const void *data) override
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
}