#include "Globals.hpp"
#include "Players.hpp"

namespace Blam::Game
{
	Data::DataArray<PlayerDatum> &GetPlayers()
	{
		return *AnvilCommon::GetThreadStorage<Data::DataArray<PlayerDatum>>(0x40);
	}

	Data::DatumIndex GetLocalPlayer(const int32_t p_Index)
	{
		typedef uint32_t(*GetLocalPlayerPtr)(int index);
		auto GetLocalPlayer = reinterpret_cast<GetLocalPlayerPtr>(0x589C30);
		return GetLocalPlayer(p_Index);
	}

	void PlayerPropertiesExtender::Add(std::shared_ptr<PlayerPropertiesExtensionBase> extension)
	{
		extensions.push_back(extension);
	}

	size_t PlayerPropertiesExtender::GetTotalSize()
	{
		size_t result = 0;
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
			result += (*it)->GetDataSize();
		return result;
	}

	void PlayerPropertiesExtender::BuildData(int32_t playerIndex, void *out)
	{
		// Write all of the data structures in order
		uint8_t *ptr = static_cast<uint8_t*>(out);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->BuildData(playerIndex, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	void PlayerPropertiesExtender::ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const void *data)
	{
		// Apply all of the data structures in order
		const uint8_t *ptr = static_cast<const uint8_t*>(data);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->ApplyData(playerIndex, properties, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	void PlayerPropertiesExtender::SerializeData(Blam::Data::BitStream *stream, const void *data)
	{
		// Serialize all of the structures in order
		const uint8_t *ptr = static_cast<const uint8_t*>(data);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->Serialize(stream, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	void PlayerPropertiesExtender::DeserializeData(Blam::Data::BitStream *stream, void *out)
	{
		// Deserialize all of the structures in order
		uint8_t *ptr = static_cast<uint8_t*>(out);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->Deserialize(stream, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	const size_t PlayerPropertiesPacketHeaderSize = 0x18;
	const size_t PlayerPropertiesSize = 0x30;
	const size_t PlayerPropertiesPacketFooterSize = 0x4;

	size_t GetPlayerPropertiesPacketSize()
	{
		static size_t size;

		if (size == 0)
		{
			// TODO: Fix
			size_t extensionSize = 0; // PlayerPropertiesExtender::Instance()->GetTotalSize();
			size = PlayerPropertiesPacketHeaderSize + PlayerPropertiesSize + extensionSize + PlayerPropertiesPacketFooterSize;
		}

		return size;
	}
}