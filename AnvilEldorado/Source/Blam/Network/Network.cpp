#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#include <timeapi.h>
#include "Engine.hpp"
#include "Network.hpp"

namespace Blam::Network
{
	Session *GetActiveSession()
	{
		return *reinterpret_cast<Session **>((uint8_t *)GetModuleBase() + 0x15AB848);
	}

	const auto SessionMembership_FindFirstPeer = reinterpret_cast<int(__thiscall *)(const SessionMembership *)>(0x44E690);
	int SessionMembership::FindFirstPeer() const
	{
		return SessionMembership_FindFirstPeer(this);
	}

	const auto SessionMembership_FindNextPeer = reinterpret_cast<int(__thiscall *)(const SessionMembership *, int32_t)>(0x44E710);
	int SessionMembership::FindNextPeer(int p_LastPeer) const
	{
		return SessionMembership_FindNextPeer(this, p_LastPeer);
	}

	const auto SessionMembership_FindFirstPlayer = reinterpret_cast<int(__thiscall *)(const SessionMembership *)>(0x44E6C0);
	int SessionMembership::FindFirstPlayer() const
	{
		return SessionMembership_FindFirstPlayer(this);
	}

	const auto SessionMembership_FindNextPlayer = reinterpret_cast<int(__thiscall *)(const SessionMembership *, int32_t)>(0x44E750);
	int SessionMembership::FindNextPlayer(int p_LastPlayer) const
	{
		return SessionMembership_FindNextPlayer(this, p_LastPlayer);
	}

	const auto SessionMembership_GetPeerPlayer = reinterpret_cast<int(*)(const uint32_t *, int32_t)>(0x52E280);
	int SessionMembership::GetPeerPlayer(int p_Peer) const
	{
		return SessionMembership_GetPeerPlayer(Peers[p_Peer].PlayerMasks, 16);
	}

	int SessionMembership::GetPlayerPeer(int p_Player) const
	{
		if (p_Player < 0 || p_Player >= MaxPlayers)
			return -1;

		return PlayerSessions[p_Player].PeerIndex;
	}

	int SessionMembership::GetPeerTeam(int p_Peer) const
	{
		auto s_PlayerIndex = GetPeerPlayer(p_Peer);

		if (s_PlayerIndex < 0)
			return -1;

		return PlayerSessions[s_PlayerIndex].Properties.TeamIndex;
	}

	void SessionMembership::Update()
	{
		// The engine does this all over the place
		// These properties are almost always incremented together, so it's hard to tell what they actually do individually
		PlayerUpdateCount++;
		PeerUpdateCount++;
	}

	const auto Observer_ObserverChannelSendMessage = reinterpret_cast<int(__thiscall *)(Observer *, int32_t, int32_t, bool, int, int, const void *)>(0x4474F0);
	void Observer::ObserverChannelSendMessage(int ownerIndex, int channelIndex, bool secure, int id, int packetSize, const void *packet)
	{
		Observer_ObserverChannelSendMessage(this, ownerIndex, channelIndex, secure, id, packetSize, packet);
	}

	PingPacket MakePingPacket(uint16_t id)
	{
		PingPacket packet;
		packet.ID = id;
		packet.QosResponse = false;
		packet.Timestamp = timeGetTime();
		return packet;
	}

	void Observer::Ping(int ownerIndex, int channelIndex, uint16_t id)
	{
		auto packet = MakePingPacket(id);
		ObserverChannelSendMessage(ownerIndex, channelIndex, true, ePacketIDPing, sizeof(packet), &packet);
	}

	const auto PacketHeader_Initialize = reinterpret_cast<void(*)(int32_t, PacketHeader *)>(0x482040);
	PacketHeader::PacketHeader()
	{
		auto session = GetActiveSession();
		if (!session)
		{
			memset(this, 0, sizeof(*this));
			return;
		}

		PacketHeader_Initialize(session->AddressIndex, this);
	}

	const auto Session_GetChannelIndex = reinterpret_cast<int(__thiscall *)(const Observer *, uint32_t, const ObserverChannel *)>(0x447150);
	const auto Session_GetChannelPeer = reinterpret_cast<int(__thiscall *)(const SessionMembership *, int32_t)>(0x44E860);
	int Session::GetChannelPeer(const ObserverChannel *channel) const
	{
		// Look up the channel index
		auto index = Session_GetChannelIndex(Observer, Unknown10, channel);
		if (index < 0)
			return index;

		// Use the channel index to look up the peer
		return Session_GetChannelPeer(&MembershipInfo, index);
	}

	bool Session::HasTeams() const
	{
		auto gameVariantParameter = &Parameters.GameVariant;
		if (!gameVariantParameter->IsAvailable())
			return false;

		auto gameVariant = gameVariantParameter->Get();
		if (!gameVariant)
			return false;

		return (gameVariant->TeamGame & 1) != 0;
	}

	NetworkAddress Session::GetPeerAddress(int peerIndex) const
	{
		if (peerIndex != MembershipInfo.LocalPeerIndex)
		{
			auto channelIndex = MembershipInfo.PeerChannels[peerIndex].ChannelIndex;
			if (channelIndex >= 0)
				return Observer->Channels[channelIndex].Address;
		}

		return NetworkAddress::FromInAddr(0x0100007F, 11774);
	}

	const auto SessionParameters_SetSessionMode = reinterpret_cast<bool(__thiscall *)(SessionParameters *, int32_t)>(0x459A40);
	bool SessionParameters::SetSessionMode(int mode)
	{
		return SessionParameters_SetSessionMode(this, mode);
	}

	bool SessionParameter::IsAvailable() const
	{
		typedef bool(__thiscall *IsAvailablePtr)(const SessionParameter *thisPtr);
		auto IsAvailable = reinterpret_cast<IsAvailablePtr>(0x450CF0);
		return IsAvailable(this);
	}

	const auto Game_GetGameVariant = reinterpret_cast<Game::GameVariant *(__thiscall *)(const GameVariantSessionParameter *)>(0x456140);
	Game::GameVariant *GameVariantSessionParameter::Get() const
	{
		return Game_GetGameVariant(this);
	}

	const auto Game_GetMapVariant = reinterpret_cast<void*(__thiscall *)(const MapVariantSessionParameter *)>(0x456410);
	void* MapVariantSessionParameter::Get() const
	{
		return Game_GetMapVariant(this);
	}

	PacketTable *GetPacketTable()
	{
		return *reinterpret_cast<PacketTable **>((uint8_t *)GetModuleBase() + 0x1E4A498);
	}

	void SetPacketTable(PacketTable *newTable)
	{
		*reinterpret_cast<PacketTable **>((uint8_t *)GetModuleBase() + 0x1E4A498) = newTable;
	}

	const auto PacketTable_RegisterPacket = reinterpret_cast<void(__thiscall *)(PacketTable *, int32_t, const char *, int32_t, int32_t, int32_t, SerializePacketFn, DeserializePacketFn, int32_t, int32_t)>(0x4801B0);
	void PacketTable::Register(int index, const char *name, int unk8, int minSize, int maxSize, SerializePacketFn serializeFunc, DeserializePacketFn deserializeFunc, int unk1C, int unk20)
	{
		PacketTable_RegisterPacket(this, index, name, unk8, minSize, maxSize, serializeFunc, deserializeFunc, unk1C, unk20);
	}

	const auto MessageGateway_SendDirectedMessage = reinterpret_cast<bool(__thiscall *)(MessageGateway *, const NetworkAddress &, int32_t, int32_t, const void *)>(0x4840C0);
	bool MessageGateway::SendDirectedMessage(const NetworkAddress &address, int id, int packetSize, const void *packet)
	{
		return MessageGateway_SendDirectedMessage(this, address, id, packetSize, packet);
	}

	bool MessageGateway::Ping(NetworkAddress &address, uint16_t id)
	{
		auto packet = MakePingPacket(id);
		return SendDirectedMessage(address, ePacketIDPing, sizeof(packet), &packet);
	}

	const auto Network_SquadSession_BootPlayer = reinterpret_cast<bool(__cdecl *)(int32_t, int32_t)>(0x437D60);
	bool BootPlayer(int playerIndex, int reason)
	{
		return Network_SquadSession_BootPlayer(playerIndex, reason);
	}

	const auto Network_SquadSession_EndGame = reinterpret_cast<bool(__cdecl *)()>(0x438780);
	bool EndGame()
	{
		return Network_SquadSession_EndGame();
	}

	const auto Network_LeaveGame = reinterpret_cast<void(__cdecl *)()>(0xA81270);
	void LeaveGame()
	{
		Network_LeaveGame();
	}

	bool NetworkAddress::Parse(const std::string &addr, uint16_t port, NetworkAddress *result)
	{
		struct in_addr inAddr;
		if (!inet_pton(AF_INET, addr.c_str(), &inAddr))
			return false;

		*result = NetworkAddress::FromInAddr(inAddr.S_un.S_addr, port);

		return true;
	}

	std::string NetworkAddress::ToString() const
	{
		struct in_addr inAddr;
		inAddr.S_un.S_addr = ToInAddr();

		char ipStr[INET_ADDRSTRLEN];
		if (!inet_ntop(AF_INET, &inAddr, ipStr, sizeof(ipStr)))
			return "";

		return std::string(ipStr);
	}
}