#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>

#include <sstream>

#include "BuildInfo.hpp"
#include "Globals.hpp"
#include "Utils\Logger.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"

#include "Network.hpp"

namespace AnvilEldorado
{
	char *GetIPStringFromInAddrHook(void *p_InAddr)
	{
		static char s_IpAddrStr[64];
		memset(s_IpAddrStr, 0, 64);

		auto s_Ip3 = *(uint8_t *)p_InAddr;
		auto s_Ip2 = *((uint8_t *)p_InAddr + 1);
		auto s_Ip1 = *((uint8_t *)p_InAddr + 2);
		auto s_Ip0 = *((uint8_t *)p_InAddr + 3);

		auto s_Port = *(uint16_t *)((uint8_t *)p_InAddr + 0x10);
		auto s_Type = *(uint16_t *)((uint8_t *)p_InAddr + 0x12);

		sprintf_s(s_IpAddrStr, 64, "%hd.%hd.%hd.%hd:%hd (%hd)", s_Ip0, s_Ip1, s_Ip2, s_Ip3, s_Port, s_Type);

		return s_IpAddrStr;
	}

	char XnAddrToInAddrHook(void *pxna, void *pxnkid, void *in_addr)
	{
		//printf("XnAddrToInAddrHook...");
		uint32_t maxMachines = *(uint32_t*)(0x228E6D8);
		uint8_t *syslinkDataPtr = (uint8_t*)*(uint32_t*)(0x228E6D8 + 0x4);

		for (uint32_t i = 0; i < maxMachines; i++)
		{
			uint8_t *entryPtr = syslinkDataPtr;
			syslinkDataPtr += 0x164F8;
			uint8_t entryStatus = *entryPtr;
			if (entryStatus == 0)
				continue;

			uint8_t *xnkidPtr = entryPtr + 0x9E;
			uint8_t *xnaddrPtr = entryPtr + 0xAE;
			uint8_t *ipPtr = entryPtr + 0x170;
			uint8_t *portPtr = entryPtr + 0x174;

			if (memcmp(pxna, xnaddrPtr, 0x10) == 0 && memcmp(pxnkid, xnkidPtr, 0x10) == 0)
			{
				// in_addr struct:
				// 0x0 - 0x10: IP (first 4 bytes for IPv4, 0x10 for IPv6)
				// 0x10 - 0x12: Port number
				// 0x12 - 0x14: IP length (4 for IPv4, 0x10 for IPv6)

				memset(in_addr, 0, 0x14);
				memcpy(in_addr, ipPtr, 4);
				memcpy((uint8_t*)in_addr + 0x10, portPtr, 2);

				*(uint16_t*)((uint8_t*)in_addr + 0x12) = 4;

				return 1;
			}
		}

		typedef char(*Network_XnAddrToInAddrFunc)(void *pxna, void *pxnkid, void *in_addr);
		Network_XnAddrToInAddrFunc XnAddrToInAddr = (Network_XnAddrToInAddrFunc)0x52D840;
		return XnAddrToInAddr(pxna, pxnkid, in_addr);
	}

	char InAddrToXnAddrHook(void *ina, void  *pxna, void  *pxnkid)
	{
		uint32_t maxMachines = *(uint32_t*)(0x228E6D8);
		uint8_t *syslinkDataPtr = (uint8_t*)*(uint32_t*)(0x228E6DC);

		for (uint32_t i = 0; i < maxMachines; i++)
		{
			uint8_t *entryPtr = syslinkDataPtr;
			syslinkDataPtr += 0x164F8;
			uint8_t entryStatus = *entryPtr;
			if (entryStatus == 0)
				continue;

			uint8_t *xnkidPtr = entryPtr + 0x9E;
			uint8_t *xnaddrPtr = entryPtr + 0xAE;
			uint8_t *ipPtr = entryPtr + 0x170;

			if (memcmp(ipPtr, ina, 0x4) == 0)
			{
				memcpy(pxna, xnaddrPtr, 0x10);
				memcpy(pxnkid, xnkidPtr, 0x10);

				return 1;
			}
		}

		typedef char(*Network_InAddrToXnAddrFunc)(void *ina, void  *pxna, void  *pxnkid);
		Network_InAddrToXnAddrFunc InAddrToXnAddr = (Network_InAddrToXnAddrFunc)0x52D840;
		return InAddrToXnAddr(ina, pxna, pxnkid);
	}

	const auto Network_ManagedSession_CreateSessionInternal = reinterpret_cast<DWORD(__cdecl *)(int32_t, int32_t)>(0x481550);
	DWORD __cdecl Network_ManagedSession_CreateSessionInternal_Hook(int32_t a1, int32_t a2)
	{
		auto *s_Network = Network::Instance();

		DWORD isOnline = *(DWORD*)a2;
		bool isHost = (*(uint16_t *)(a2 + 284) & 1);

		auto retval = Network_ManagedSession_CreateSessionInternal(a1, a2);

		if (!isHost)
			return retval;

		if (isOnline == 1)
		{
			/*
			auto& voipvars = Modules::ModuleVoIP::Instance();
			if (voipvars.VarVoIPServerEnabled->ValueInt == 1) {
			//Start the Teamspeak VoIP Server since this is the host
			CreateThread(0, 0, StartTeamspeakServer, 0, 0, 0);

			if (voipvars.VarVoIPEnabled->ValueInt == 1)
			{
			//Make sure teamspeak is stopped before we try to start it.
			StopTeamspeakClient();
			//Join the Teamspeak VoIP Server so the host can talk
			CreateThread(0, 0, StartTeamspeakClient, 0, 0, 0);
			}
			}*/

			// TODO: give output if StartInfoServer fails
			s_Network->StartServer();
		}
		else
		{
			s_Network->StopServer();
			/* TODO:
			//Stop the VoIP Server and client
			StopTeamspeakClient();
			StopTeamspeakServer();*/
		}

		return retval;
	}

	// ASCII chars that can't appear in names
	const wchar_t DisallowedNameChars[] = { '\'', '\"', '<', '>', '/', '\\' };

	void SanitizePlayerName(wchar_t *name)
	{
		int32_t i, dest = 0;
		auto space = false;
		for (i = 0; i < 15 && dest < 15 && name[i]; i++)
		{
			auto allowed = false;
			if (name[i] > 32 && name[i] < 127)
			{
				// ASCII characters are allowed if they aren't in the disallowed list
				allowed = true;
				for (auto ch : DisallowedNameChars)
				{
					if (name[i] == ch)
					{
						allowed = false;
						break;
					}
				}
			}
			else if (name[i] == ' ' && dest > 0)
			{
				// If this isn't at the beginning of the string, indicate that
				// a space should be inserted before the next allowed character
				space = true;
			}
			if (allowed)
			{
				if (space && dest < 14)
				{
					name[dest++] = ' ';
					space = false;
				}
				name[dest++] = name[i];
			}
		}
		memset(&name[dest], 0, (16 - dest) * sizeof(wchar_t));
		if (dest == 0)
			wcscpy_s(name, 16, L"Forgot");
	}

	const auto ApplyPlayerProperties = reinterpret_cast<void(__thiscall *)(void *, int32_t, uint32_t, uint32_t, void *, uint32_t)>(0x450890);

	// Applies player properties data including extended properties
	void __fastcall ApplyPlayerPropertiesExtendedHook(Blam::Network::SessionMembership *thisPtr, void *unused, int32_t playerIndex, uint32_t arg4, uint32_t arg8, uint8_t *data, uint32_t arg10)
	{
		auto properties = &thisPtr->PlayerSessions[playerIndex].Properties;

		// If the player already has a name and isn't host, then use that instead of the one in the packet
		// This prevents people from changing their name mid-game by forcing a player properties update
		// The player name is stored at the beginning of the player-properties packet (TODO: Map it out properly!)
		if (properties->DisplayName[0] && !thisPtr->Peers[thisPtr->HostPeerIndex].OwnsPlayer(playerIndex))
			memcpy(reinterpret_cast<wchar_t*>(data), properties->DisplayName, sizeof(properties->DisplayName));
		else
			SanitizePlayerName(reinterpret_cast<wchar_t*>(data));

		// Apply the base properties
		ApplyPlayerProperties(thisPtr, playerIndex, arg4, arg8, data, arg10);

		// Apply the extended properties
		Blam::Game::PlayerPropertiesExtender::Instance()->ApplyData(playerIndex, properties, data + Blam::Game::PlayerPropertiesSize);
		//Server::Voting::PlayerJoinedVoteInProgress(playerIndex); //TODO find somewhere else to put this.
	}

	// This completely replaces c_network_session::peer_request_player_desired_properties_update
	// Editing the existing function doesn't allow for a lot of flexibility
	bool __fastcall PeerRequestPlayerDesiredPropertiesUpdateHook(Blam::Network::Session *thisPtr, void *unused, uint32_t arg0, uint32_t arg4, void *properties, uint32_t argC)
	{
		if (thisPtr->Type == 3)
			return false;

		// Ensure that there is a player associated with the local peer
		auto membership = &thisPtr->MembershipInfo;
		auto playerIndex = thisPtr->MembershipInfo.GetPeerPlayer(membership->LocalPeerIndex);
		if (playerIndex == -1)
			return false;

		// Copy the player properties to a new array and add the extension data
		auto packetSize = Blam::Game::GetPlayerPropertiesPacketSize();
		auto extendedSize = packetSize - Blam::Game::PlayerPropertiesPacketHeaderSize - Blam::Game::PlayerPropertiesPacketFooterSize;
		auto extendedProperties = std::make_unique<uint8_t[]>(extendedSize);
		memcpy(&extendedProperties[0], properties, Blam::Game::PlayerPropertiesSize);
		Blam::Game::PlayerPropertiesExtender::Instance()->BuildData(playerIndex, &extendedProperties[Blam::Game::PlayerPropertiesSize]);

		if (thisPtr->Type == 6 || thisPtr->Type == 7)
		{
			// Apply player properties locally
			ApplyPlayerPropertiesExtendedHook(membership, nullptr, playerIndex, arg0, arg4, &extendedProperties[0], argC);
		}
		else
		{
			// Send player properties across the network
			auto hostPeer = membership->HostPeerIndex;
			auto channelIndex = membership->PeerChannels[hostPeer].ChannelIndex;
			if (channelIndex == -1)
				return true;

			// Allocate the packet
			auto packet = std::make_unique<uint8_t[]>(packetSize);
			memset(&packet[0], 0, packetSize);

			// Initialize it
			typedef void(*InitPacketPtr)(int32_t id, void *packet);
			InitPacketPtr InitPacket = reinterpret_cast<InitPacketPtr>(0x482040);
			InitPacket(thisPtr->AddressIndex, &packet[0]);

			// Set up the header and footer
			*reinterpret_cast<int*>(&packet[0x10]) = arg0;
			*reinterpret_cast<uint32_t*>(&packet[0x14]) = arg4;
			*reinterpret_cast<uint32_t*>(&packet[packetSize - Blam::Game::PlayerPropertiesPacketFooterSize]) = argC;

			// Copy the player properties structure in
			memcpy(&packet[Blam::Game::PlayerPropertiesPacketHeaderSize], &extendedProperties[0], extendedSize);

			// Send!
			thisPtr->Observer->ObserverChannelSendMessage(thisPtr->Unknown10, channelIndex, 0, 0x1A, packetSize, &packet[0]);
		}
		return true;
	}

	const auto RegisterPacket = reinterpret_cast<void(__thiscall *)(void *, int32_t, const char *, int32_t, int32_t, int32_t, void *, void *, int32_t, int32_t)>(0x4801B0);

	// Changes the size of the player-properties packet to include extension data
	void __fastcall RegisterPlayerPropertiesPacketHook(void *thisPtr, void *unused, int32_t packetId, const char *packetName, int32_t arg8, int32_t size1, int32_t size2, void *serializeFunc, void *deserializeFunc, int32_t arg1C, int32_t arg20)
	{
		size_t newSize = Blam::Game::GetPlayerPropertiesPacketSize();
		RegisterPacket(thisPtr, packetId, packetName, arg8, newSize, newSize, serializeFunc, deserializeFunc, arg1C, arg20);
	}

	const auto SerializePlayerProperties = reinterpret_cast<void(*)(Blam::Data::BitStream *, uint8_t *, bool)>(0x4433C0);

	// Serializes extended player-properties data
	void SerializePlayerPropertiesHook(Blam::Data::BitStream *stream, uint8_t *buffer, bool flag)
	{
		// Serialize base data
		SerializePlayerProperties(stream, buffer, flag);

		// Serialize extended data
		Blam::Game::PlayerPropertiesExtender::Instance()->SerializeData(stream, buffer + Blam::Game::PlayerPropertiesSize);
	}

	const auto DeserializePlayerProperties = reinterpret_cast<bool(*)(Blam::Data::BitStream *, uint8_t *, bool)>(0x4432E0);

	// Deserializes extended player-properties data
	bool DeserializePlayerPropertiesHook(Blam::Data::BitStream *stream, uint8_t *buffer, bool flag)
	{
		// Deserialize base data
		bool succeeded = DeserializePlayerProperties(stream, buffer, flag);

		// Deserialize extended data
		if (succeeded)
			Blam::Game::PlayerPropertiesExtender::Instance()->DeserializeData(stream, buffer + Blam::Game::PlayerPropertiesSize);

		return succeeded;
	}

	const auto Network_Leader_RequestBootMachine = reinterpret_cast<bool(__thiscall *)(void *, void *, int32_t)>(0x45D4A0);

	bool __fastcall RequestBootMachineHook(void *thisPtr, void *unused, Blam::Network::PeerInfo *peer, int32_t reason)
	{
		auto session = Blam::Network::Session::Current();
		auto membership = &session->MembershipInfo;
		auto peerIndex = peer - membership->Peers;
		auto playerIndex = membership->GetPeerPlayer(peerIndex);

		if (!Network_Leader_RequestBootMachine(thisPtr, peer, reason))
			return false;

		/* TODO: Boot the player from VoIP
		std::string playerName;
		if (playerIndex >= 0)
		playerName = Utils::String::ThinString(membership->PlayerSessions[playerIndex].Properties.DisplayName);
		if (playerIndex >= 0)
		kickTeamspeakClient(playerName);
		*/

		return true;
	}

	void PongReceivedImpl(const Blam::Network::NetworkAddress &p_From, const Blam::Network::PongPacket &p_Pong, uint32_t p_Latency)
	{
		Network::Instance()->OnPongReceived(p_From, p_Pong, p_Latency);
	}

	__declspec(naked) void PongReceivedHook()
	{
		__asm
		{
			push esi // Latency
			push edi // Pong packet
			push dword ptr[ebp + 8] // Sender
			call PongReceivedImpl
			add esp, 12
			push 0x49D9FA
			ret
		}
	}

	void LifeCycleStateChangedImpl(Blam::Network::LifeCycleState p_NewState)
	{
		Network::Instance()->OnLifeCycleStateChanged(p_NewState);
	}

	__declspec(naked) void LifeCycleStateChangedHook()
	{
		__asm
		{
			pop esi // HACK: esi = return address

					// Execute replaced code
					mov ecx, edi // ecx = New lifecycle state object
					call dword ptr[eax + 8] // lifecycle->enter()

					push dword ptr[ebx] // Lifecycle state type
					call LifeCycleStateChangedImpl
					add esp, 4
					jmp esi
		}
	}

	bool __fastcall Network_Session_HandleJoinRequest_Hook(Blam::Network::Session *p_This, void *p_Unused, const Blam::Network::NetworkAddress &p_Address, void *p_Request)
	{
		/* TODO:
		// Convert the IP to a string
		struct in_addr s_InAddr;
		s_InAddr.S_un.S_addr = p_Address.ToInAddr();
		char ipStr[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &s_InAddr, ipStr, sizeof(ipStr)))
		{
			// Check if the IP is in the ban list
			auto banList = Server::LoadDefaultBanList();
			if (banList.ContainsIp(ipStr))
			{
				// Send a join refusal
				typedef void(__thiscall *Network_session_acknowledge_join_requestFunc)(Blam::Network::Session *thisPtr, const Blam::Network::NetworkAddress &address, int32_t reason);
				auto Network_session_acknowledge_join_request = reinterpret_cast<Network_session_acknowledge_join_requestFunc>(0x45A230);
				Network_session_acknowledge_join_request(thisPtr, address, 0); // TODO: Use a special code for bans and hook the join refusal handler so we can display a message to the player
				Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Refused join request from banned IP %s", ipStr);
				return true;
			}
		}*/

		// Continue the join process
		typedef bool(__thiscall *Network_session_handle_join_requestFunc)(Blam::Network::Session *thisPtr, const Blam::Network::NetworkAddress &address, void *request);
		auto Network_session_handle_join_request = reinterpret_cast<Network_session_handle_join_requestFunc>(0x4DA410);
		return Network_session_handle_join_request(p_This, p_Address, p_Request);
	}

	const auto Network_State_EndGame_WriteStatsEnter = reinterpret_cast<char(__thiscall *)(void *, int32_t, int32_t, int32_t)>(0x492B50);

	char __fastcall Network_State_EndGame_WriteStatsEnter_Hook(void *thisPtr, int32_t unused, int32_t a2, int32_t a3, int32_t a4)
	{
		// There used to be a stats upload here, which is why this hook does nothing
		return Network_State_EndGame_WriteStatsEnter(thisPtr, a2, a3, a4);
	}

	const auto Network_State_LeavingEnter = reinterpret_cast<int8_t(__thiscall *)(void *, int32_t, int32_t, int32_t)>(0x4933E0);

	int8_t __fastcall Network_State_LeavingEnter_Hook(void *thisPtr, int32_t unused, int32_t a2, int32_t a3, int32_t a4)
	{
		/* TODO:
		Patches::Network::StopInfoServer();

		StopTeamspeakClient();
		StopTeamspeakServer();*/

		return Network_State_LeavingEnter(thisPtr, a2, a3, a4);
	}

	int32_t Network_GetMaxPlayers_Hook()
	{
		/* TODO:
		int32_t maxPlayers = Modules::ModuleServer::Instance().VarServerMaxPlayers->ValueInt;
		return Utils::Clamp(maxPlayers, 1, 16);*/

		return 16;
	}

	const auto Network_Link_CreateEndpoint = reinterpret_cast<bool(__cdecl *)(int32_t, int16_t, int8_t, void *)>(0x43B6F0);
	const auto Network_Sub43FED0 = reinterpret_cast<void *(__cdecl *)(SOCKET)>(0x43FED0);

	bool __fastcall Network_GetEndpoint_Hook(uint8_t *p_Data, void *p_Unused)
	{
		auto *s_Network = Network::Instance();

		auto *s_Socket = p_Data + 12;
		auto s_Port = s_Network->GetServerPort();
		auto s_Success = false;

		while (true)
		{
			*reinterpret_cast<uint32_t *>(0x1860454) = s_Port;
			s_Success = Network_Link_CreateEndpoint(0, (short)s_Port, 1, s_Socket);

			if (s_Success)
				break;

			if (*s_Socket)
			{
				Network_Sub43FED0(*s_Socket);
				*s_Socket = 0;
			}

			if (++s_Port - s_Network->GetServerPort() >= 1000)
			{
				*reinterpret_cast<uint32_t *>(0x1860454) = s_Network->GetServerPort();
				return s_Success;
			}
		}
		return s_Success;
	}

	const auto Network_Session_JoinRemoteSession = reinterpret_cast<char(__fastcall *)(void *, int32_t, int8_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, void *, int32_t, int32_t)>(0x45D1E0);

	int8_t __fastcall Network_Session_JoinRemoteSession_Hook(void *thisPtr, int32_t unused, int8_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8, int32_t a9, int32_t a10, int32_t a11, void *a12, int32_t a13, int32_t a14)
	{
		/* TODO:
		rapidjson::StringBuffer jsonBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
		jsonWriter.StartObject();
		jsonWriter.Key("connecting");
		jsonWriter.Bool(true);
		jsonWriter.Key("success");
		jsonWriter.Bool(false);
		jsonWriter.EndObject();

		Web::Ui::ScreenLayer::Notify("serverconnect", jsonBuffer.GetString(), true);*/

		return Network_Session_JoinRemoteSession(thisPtr, unused, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);;
	}

	const auto Network_Session_InitiateLeaveProtocol = reinterpret_cast<int(__fastcall *)(void *, int32_t, int8_t)>(0x45CB80);

	int32_t __fastcall Network_Session_InitiateLeaveProtocol_Hook(void *thisPtr, int32_t unused, int8_t forceClose)
	{
		/* TODO:
		rapidjson::StringBuffer jsonBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
		jsonWriter.StartObject();
		jsonWriter.Key("connecting");
		jsonWriter.Bool(false);
		jsonWriter.Key("success");
		jsonWriter.Bool(false);
		jsonWriter.EndObject();

		Web::Ui::ScreenLayer::Notify("serverconnect", jsonBuffer.GetString(), true);*/

		return Network_Session_InitiateLeaveProtocol(thisPtr, unused, forceClose);
	}

	const auto Network_Session_ParametersClear = reinterpret_cast<int(__fastcall *)(void *, int32_t)>(0x486580);

	int32_t __fastcall Network_Session_ParametersClear_Hook(void *thisPtr, int32_t unused)
	{
		/* TODO:
		rapidjson::StringBuffer jsonBuffer;
		rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
		jsonWriter.StartObject();
		jsonWriter.Key("connecting");
		jsonWriter.Bool(false);
		jsonWriter.Key("success");
		jsonWriter.Bool(true);
		jsonWriter.EndObject();

		Web::Ui::ScreenLayer::Notify("serverconnect", jsonBuffer.GetString(), true);*/

		return Network_Session_ParametersClear(thisPtr, unused);
	}

	__declspec(naked) void DedicatedServerHook()
	{
		__asm
		{
			cmp edi, 0xffffffff
			jz end
			movzx ecx, di
			mov eax, [eax + 0x40]
			imul ecx, 0x2f08
			mov eax, [eax + 0x44]
			mov edi, [eax + ecx + 0x30]

			end:
			push 0xA2E709
				ret
		}
	}

	bool PatchVersionNumbers()
	{
		using AnvilCommon::Utils::Patch;

		uint32_t s_VersionNumber = ANVIL_BUILD;
		auto *s_Version = (uint8_t *)&s_VersionNumber;

		return Patch(0x101421, { s_Version[3], s_Version[2], s_Version[1], s_Version[0] }).Apply()
			&& Patch(0x10143A, { s_Version[3], s_Version[2], s_Version[1], s_Version[0] }).Apply();
	}

	bool Network::Init()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Fix network debug strings having (null) instead of an IP address
		return Hook(0x3F6F0, GetIPStringFromInAddrHook).Apply()
			// Fix for XnAddrToInAddr to try checking syslink-menu data for XnAddr->InAddr mapping before consulting XNet
			&& Hook(0x30B6C, XnAddrToInAddrHook, HookFlags::IsCall).Apply()
			&& Hook(0x30F51, InAddrToXnAddrHook, HookFlags::IsCall).Apply()
			// Hook call to Network_managed_session_create_session_internal so we can detect when an online game is started
			&& Hook(0x82AAC, Network_ManagedSession_CreateSessionInternal_Hook, HookFlags::IsCall).Apply()
			// Patch version subs to return version of this DLL, to make people with older DLLs incompatible
			&& PatchVersionNumbers()
			// Player-properties packet hooks
			&& Hook(0x5DD20, PeerRequestPlayerDesiredPropertiesUpdateHook).Apply()
			&& Hook(0xDAF4F, ApplyPlayerPropertiesExtendedHook, HookFlags::IsCall).Apply()
			&& Hook(0xDFF7E, RegisterPlayerPropertiesPacketHook, HookFlags::IsCall).Apply()
			&& Hook(0xDFD53, SerializePlayerPropertiesHook, HookFlags::IsCall).Apply()
			&& Hook(0xDE178, DeserializePlayerPropertiesHook, HookFlags::IsCall).Apply()
			// Hook leader_request_boot_machine so we can do some extra things if the boot succeeded
			&& Hook(0x37E17, RequestBootMachineHook, HookFlags::IsCall).Apply()
			// Pong hook
			&& Hook(0x9D9DB, PongReceivedHook).Apply()
			// Lifecycle state change hook
			&& Hook(0x8E527, LifeCycleStateChangedHook, HookFlags::IsCall).Apply()
			&& Hook(0x8E10F, LifeCycleStateChangedHook, HookFlags::IsCall).Apply()
			// Hook the join request handler to check the user's IP address against the ban list
			&& Hook(0x9D0F7, Network_Session_HandleJoinRequest_Hook, HookFlags::IsCall).Apply()
			// Hook c_life_cycle_state_handler_end_game_write_stats's vftable ::entry method
			&& Hook(0x16183A0, Network_State_EndGame_WriteStatsEnter_Hook, HookFlags::IsVirtual).Apply()
			// Hook c_life_cycle_state_handler_leaving's vftable ::entry method
			&& Hook(0x16183BC, Network_State_LeavingEnter_Hook, HookFlags::IsVirtual).Apply()
			// Set the max player count to Server.MaxPlayers when hosting a lobby
			&& Hook(0x67FA0D, Network_GetMaxPlayers_Hook, HookFlags::IsCall).Apply()
			&& Hook(0x3BAFB, Network_GetEndpoint_Hook, HookFlags::IsCall).Apply()
			&& Hook(0x7F5B9, Network_Session_JoinRemoteSession_Hook, HookFlags::IsCall).Apply()
			// Hook Network_session_initiate_leave_protocol in Network_session_idle_peer_joining's error states
			// "peer join timed out waiting for secure connection to become established"
			&& Hook(0x9BFCA, Network_Session_InitiateLeaveProtocol_Hook, HookFlags::IsCall).Apply()
			// "peer join timed out waiting for initial updates"
			&& Hook(0x9C0BE, Network_Session_InitiateLeaveProtocol_Hook, HookFlags::IsCall).Apply()
			// "received initial update, clearing"
			&& Hook(0x899AF, Network_Session_ParametersClear_Hook, HookFlags::IsCall).Apply()
			// Fix dedicated servers
			&& Hook(0x62E6F3, DedicatedServerHook).Apply();
	}

	bool Network::StartServer()
	{
		if (m_ServerSocketOpen)
			return true;

		/* TODO: Server::Voting::StartNewVote(); */

		auto s_HWND = *reinterpret_cast<HWND *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x159C014);

		if (s_HWND == 0)
			return false;

		m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		SOCKADDR_IN s_BindAddr;
		s_BindAddr.sin_family = AF_INET;
		s_BindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

		auto s_Port = m_ServerPort;

		if (s_Port == *reinterpret_cast<uint32_t *>(0x1860454)) // make sure port isn't the same as game port
			s_Port++;

		s_BindAddr.sin_port = htons((u_short)s_Port);

		// open our listener socket
		while (bind(m_ServerSocket, (PSOCKADDR)&s_BindAddr, sizeof(s_BindAddr)) != 0)
		{
			s_Port++;

			if (s_Port == *reinterpret_cast<uint32_t *>(0x1860454)) // make sure port isn't the same as game port
				s_Port++;

			s_BindAddr.sin_port = htons((u_short)s_Port);

			if (s_Port > (m_ServerPort + 10))
				return false; // tried 10 ports, lets give up
		}

		m_ServerPort = s_Port;

		/* TODO: Setup UPnP
		if (Modules::ModuleUPnP::Instance().VarUPnPEnabled->ValueInt)
		{
		Modules::ModuleUPnP::Instance().UPnPForwardPort(true, port, port, "ElDewrito InfoServer");
		Modules::ModuleUPnP::Instance().UPnPForwardPort(false, Pointer(0x1860454).Read<uint32_t>(), Pointer(0x1860454).Read<uint32_t>(), "ElDewrito Game");
		Modules::ModuleUPnP::Instance().UPnPForwardPort(false, 9987, 9987, "ElDewrito VoIP");
		}*/

		WSAAsyncSelect(m_ServerSocket, s_HWND, WM_USER + 1338, FD_ACCEPT | FD_CLOSE);
		listen(m_ServerSocket, 5);
		m_ServerSocketOpen = true;

		return true;
	}

	bool Network::StopServer()
	{
		if (!m_ServerSocketOpen)
			return true;

		closesocket(m_ServerSocket);

		int32_t truth = 1;
		setsockopt(m_ServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&truth, sizeof(int));

		// TODO: Modules::CommandMap::Instance().ExecuteCommand("Server.Unannounce");

		m_ServerSocketOpen = false;
		// TODO: lastAnnounce = 0;

		return true;
	}

	std::string Network::GetServerName() const
	{
		return m_ServerName;
	}

	void Network::SetServerName(const std::string &p_ServerName)
	{
		m_ServerName = p_ServerName;
	}

	std::string Network::GetServerMessage() const
	{
		return m_ServerMessage;
	}

	void Network::SetServerMessage(const std::string &p_ServerMessage)
	{
		m_ServerMessage = p_ServerMessage;
	}

	std::string Network::GetServerPassword() const
	{
		return m_ServerPassword;
	}

	void Network::SetServerPassword(const std::string &p_ServerPassword)
	{
		m_ServerPassword = p_ServerPassword;
	}

	uint32_t Network::GetServerCountdown() const
	{
		return m_ServerCountdown;
	}

	void Network::SetServerCountdown(const uint32_t p_ServerCountdown)
	{
		m_ServerCountdown = p_ServerCountdown;
	}

	uint32_t Network::GetServerMaximumPlayers() const
	{
		return m_ServerMaximumPlayers;
	}

	void Network::SetServerMaximumPlayers(const uint32_t p_ServerMaximumPlayers)
	{
		m_ServerMaximumPlayers = p_ServerMaximumPlayers;
	}

	uint32_t Network::GetServerPort() const
	{
		return m_ServerPort;
	}

	void Network::SetServerPort(const uint32_t p_Port)
	{
		m_ServerPort = p_Port;
	}

	uint32_t Network::GetServerGamePort() const
	{
		return m_ServerGamePort;
	}

	void Network::SetServerGamePort(const uint32_t p_ServerGamePort)
	{
		m_ServerGamePort = p_ServerGamePort;
	}

	void Network::OnPongReceived(const Blam::Network::NetworkAddress &p_From, const Blam::Network::PongPacket &p_Pong, uint32_t p_Latency)
	{
		// TODO
	}

	void Network::OnLifeCycleStateChanged(const Blam::Network::LifeCycleState &p_NewState)
	{
		// TODO
	}
}