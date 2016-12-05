#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <TlHelp32.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "BuildInfo.hpp"
#include "Globals.hpp"
#include "Utils\Logger.hpp"
#include "Utils\Singleton.hpp"
#include "Utils\Util.hpp"
#include "Blam\Cache\StringIDCache.hpp"
#include "Blam\Data\DatumIndex.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Network\Network.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\Game\VFilesList.hpp"
#include "Blam\Tags\Items\Weapon.hpp"
#include "Blam\Tags\Scenario\Scenario.hpp"
#include "Engine.hpp"

using namespace AnvilEldorado;
using namespace AnvilCommon::Utils;

void *GetModuleBase()
{
	static void *s_Base = nullptr;

	if (s_Base == nullptr)
	{
		auto s_Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

		if (s_Snapshot == INVALID_HANDLE_VALUE)
			return nullptr;

		MODULEENTRY32 s_ModuleEntry;
		s_ModuleEntry.dwSize = sizeof(MODULEENTRY32);

		auto s_Module = Module32First(s_Snapshot, &s_ModuleEntry);

		CloseHandle(s_Snapshot);

		s_Base = (s_Module != 0) ? (void*)s_ModuleEntry.modBaseAddr : nullptr;
	}

	return s_Base;
}

void UnprotectMemory()
{
	size_t Offset = 0, Total = 0;

	MEMORY_BASIC_INFORMATION MemInfo;

	while (VirtualQuery((uint8_t *)GetModuleBase() + Offset, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
	{
		Offset += MemInfo.RegionSize;

		if (MemInfo.Protect == PAGE_EXECUTE_READ)
		{
			Total += MemInfo.RegionSize;
			VirtualProtect(MemInfo.BaseAddress, MemInfo.RegionSize, PAGE_EXECUTE_READWRITE, &MemInfo.Protect);
		}
	}
}

typedef std::function<void(HWND window)> CreateWindowCallback;
std::vector<CreateWindowCallback> CreateWindowCallbacks;

const auto UI_CreateGameWindow = reinterpret_cast<HWND(*)()>(0xA223F0);
HWND CreateGameWindowHook()
{
	auto hwnd = UI_CreateGameWindow();
	if (!hwnd)
		return nullptr;

	for (auto &&callback : CreateWindowCallbacks)
		callback(hwnd);

	return hwnd;
}

void WindowTitleSprintfHook(char *p_DestBuf, char *p_Format, char *p_Version)
{
	auto s_WindowTitle = AnvilCommon::g_BuildInfo;
	strcpy_s(p_DestBuf, 0x40, s_WindowTitle.c_str());
}

void ResolutionChangeHook()
{
	// Update the ingame UI's resolution
	typedef void(__thiscall *ApplyResolutionChangeFunc)();
	ApplyResolutionChangeFunc ApplyResolutionChange = reinterpret_cast<ApplyResolutionChangeFunc>(0xA226D0);
	ApplyResolutionChange();

	//
	// TODO: Apply new resolution to any subsystems...
	//
}

double AspectRatioHook()
{
	auto *gameResolution = reinterpret_cast<int *>(0x19106C0);
	return ((double)gameResolution[0] / (double)gameResolution[1]);
}

__declspec(naked) void FovHook()
{
	__asm
	{
		// Override the FOV that the memmove before this sets
		mov eax, ds:[0x189D42C]
		mov ds : [0x2301D98], eax
		mov ecx, [edi + 0x18]
		push 0x50CA08
		ret
	}
}

__declspec(naked) void FmodSystemInitHook()
{
	__asm
	{
		push	0; extradriverdata
		push	ebx; flags
		push	100h; maxchannels
		push	eax; FMOD_SYSTEM
		call	dword ptr[ecx]; FMOD::System::init
		push	0x404EA4
		ret
	}
}

__declspec(naked) void FmodSystemInitHook2()
{
	__asm
	{
		push	0; extradriverdata
		push	ebx; flags
		push	100h; maxchannels
		push	eax; FMOD_SYSTEM
		call	dword ptr[ecx]; FMOD::System::init
		push	0x404EC8
		ret
	}
}

void TagsLoadedImpl()
{
}

__declspec(naked) void TagsLoadedHook()
{
	__asm
	{
		call TagsLoadedImpl
		push 0x6D617467
		push 0x5030EF
		ret
	}
}

void GrenadeLoadoutImpl(uint8_t *unit)
{
	// Based off of 0x8227B48C in H3 non-TU

	// TODO: Clean this up, hardcoded offsets are hacky
	const size_t GrenadeCountOffset = 0x320;
	const size_t ControllingPlayerOffset = 0x198;
	auto grenadeCounts = unit + GrenadeCountOffset; // 0 = frag, 1 = plasma, 2 = spike, 3 = firebomb
	auto playerIndex = *reinterpret_cast<int16_t*>(unit + ControllingPlayerOffset);
	if (playerIndex < 0)
	{
		memset(grenadeCounts, 0, 4);
		return;
	}

	// Get the player's grenade setting
	auto &players = Blam::Game::GetPlayers();
	auto grenadeSetting = players[playerIndex].SpawnGrenadeSetting;

	// Get the current scenario tag
	auto scenario = Blam::Tags::Scenario::GetCurrentScenario();

	// If the setting is none (2) or the scenario has invalid starting
	// profile data, set the grenade counts to 0 and return
	if (grenadeSetting == 2 || !scenario->StartingProfile)
	{
		memset(grenadeCounts, 0, 4);
		return;
	}

	// Load the grenade counts from the scenario tag
	auto profile = &scenario->StartingProfile[0];
	grenadeCounts[0] = profile->FragGrenades;
	grenadeCounts[1] = profile->PlasmaGrenades;
	grenadeCounts[2] = profile->SpikeGrenades;
	grenadeCounts[3] = profile->FirebombGrenades;
}

__declspec(naked) void GrenadeLoadoutHook()
{
	__asm
	{
		push edi // Unit object data
		call GrenadeLoadoutImpl
		add esp, 4
		push 0x5A32C7
		ret
	}
}

void GameTickHook()
{
	//
	// TODO: Call per-tick updates here...
	//

	// Call replaced function
	reinterpret_cast<void(*)()>(0x5547F0)();
}

void ShutdownHook()
{
	//
	// TODO: Call any shutdown functions...
	//

	// Call the replaced function
	reinterpret_cast<void(*)()>(0x42E410)();
}

bool LoadMapHook(void *data)
{
	if (!reinterpret_cast<bool(*)(void *)>(0x566EF0)(data))
		return false;

	//
	// TODO: Call map-loading callbacks...
	//

	return true;
}

__declspec(naked) void AimAssistHook()
{
	__asm
	{
		// Check if the player is using a mouse
		mov edx, 0x244DE98
		mov edx, [edx]
		test edx, edx
		jnz controller

		// Set magnetism angle to 0
		xor edx, edx
		mov[edi + 0x14], edx

		// Skip past magnetism angle code
		mov edx, 0x58AA23
		jmp edx

		controller :
		// Load magnetism angle normally
		movss xmm0, dword ptr[ebx + 0x388]
			mov edx, 0x58AA1F
			jmp edx
	}
}

uint32_t DualAimAssistHook(uint32_t unitObject, short weaponIndex)
{
	// If using a mouse, report that there's no weapon being dual wielded
	if (!(*reinterpret_cast<uint32_t *>(0x244DE98)))
		return 0xFFFFFFFF;

	// Otherwise, get the weapon datum index normally
	return reinterpret_cast<uint32_t(*)(uint32_t, short)>(0xB454D0)(unitObject, weaponIndex);
}

void EndGameHook()
{
	auto session = Blam::Network::GetActiveSession();
	if (!session || !session->IsEstablished())
		return;
	if (session->IsHost())
		Blam::Network::EndGame();
	else
		Blam::Network::LeaveGame();
}

//
// TODO: Find a better place for the dialog variables below...
//

bool DialogShow;
uint32_t DialogStringId;
int32_t DialogArg1;
int32_t DialogFlags;
uint32_t DialogParentStringId;
void *UIData = 0;

int ShowHalo3PauseMenuHook(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
{
	UIData = nullptr;
	DialogStringId = 0x10084;
	DialogArg1 = 0;
	DialogFlags = 4;
	DialogParentStringId = 0x1000C;
	DialogShow = true;

	return 1;
}

void __fastcall MenuUpdateHook(void *a1, int unused, int menuIdToLoad)
{
	/*auto& dorito = ElDorito::Instance();
	if (menuIdToLoad == 0x10083)
	dorito.OnMainMenuShown();*/

	bool shouldUpdate = *(DWORD*)((uint8_t*)a1 + 0x10) >= 0x1E;
	int uiData0x18Value = 1;
	//if (menuIdToLoad == 0x100A8) // TODO1: find what 0x100A8(H3E) stringid is in HO
	//	uiData0x18Value = 5;

	reinterpret_cast<void(__thiscall *)(void *, int32_t)>(0xADF6E0)(a1, menuIdToLoad);

	if (shouldUpdate)
	{
		void *UIData = reinterpret_cast<void *(__cdecl *)(int)>(0xAB4ED0)(0x3C);

		// fill UIData with proper data
		reinterpret_cast<void *(__thiscall *)(void *, uint32_t, int32_t, int32_t, uint32_t)>(0xA92780)(UIData, menuIdToLoad, 0xFF, 4, 0x1000D);

		// post UI message
		reinterpret_cast<int(*)(void *)>(0xA93450)(UIData);

		*(uint32_t*)((char*)UIData + 0x18) = uiData0x18Value;
	}
}

__declspec(naked) void LobbyMenuButtonHandlerHook()
{
	__asm
	{
		// call sub that handles showing game options
		mov ecx, esi
		push[edi + 0x10]
		mov eax, 0xB225B0
		call eax
		// jump back to original function
		mov eax, 0xB21B9F
		jmp eax
	}
}

const size_t MaxStringLength = 0x400;

bool LocalizedStringImpl(int tagIndex, int stringId, wchar_t *outputBuffer)
{
	switch (stringId)
	{
	case 0x1010A: // start_new_campaign
	{
		// Get the version string, convert it to uppercase UTF-16, and return it
		auto s_BuildInfo = std::string("  ANVIL: ONLINE  ") + std::string("DEV BUILD ") + std::to_string(ANVIL_BUILD);
		auto s_WideBuildInfo = std::wstring(s_BuildInfo.begin(), s_BuildInfo.end());
		swprintf(outputBuffer, MaxStringLength, s_WideBuildInfo.c_str());
		return true;
	}
	}

	return false;
}

__declspec(naked) void LocalizedStringHook()
{
	__asm
	{
		// Run the hook implementation function and fallback to the original if it returned false
		push ebp
		mov ebp, esp
		push[ebp + 0x10]
		push[ebp + 0xC]
		push[ebp + 0x8]
		call LocalizedStringImpl
		add esp, 0xC
		test al, al
		jz fallback

		// Don't run the original function
		mov esp, ebp
		pop ebp
		ret

		fallback :
		// Execute replaced code and jump back to original function
		sub esp, 0x800
			mov edx, 0x51E049
			jmp edx
	}
}

const auto UI_CreateLobby = reinterpret_cast<bool(*)(int32_t)>(0xA7EE70);
bool MainMenuCreateLobbyHook(int lobbyType)
{
	// If matchmaking is selected, show the server browser instead
	// TODO: Really need to map out the ui_game_mode enum...
	switch (lobbyType)
	{
	case 1: // Matchmaking
			//Web::Ui::ScreenLayer::Show("browser", "{}");
		return true;
	case 4: // Theater (rip)
			//ShowLanBrowser();
		return true;
	default:
		return UI_CreateLobby(lobbyType);
	}
}

char *GetIPStringFromInAddrHook(void *inaddr)
{
	static char ipAddrStr[64];
	memset(ipAddrStr, 0, 64);

	uint8_t ip3 = *(uint8_t *)inaddr;
	uint8_t ip2 = *((uint8_t *)inaddr + 1);
	uint8_t ip1 = *((uint8_t *)inaddr + 2);
	uint8_t ip0 = *((uint8_t *)inaddr + 3);

	uint16_t port = *(uint16_t *)((uint8_t *)inaddr + 0x10);
	uint16_t type = *(uint16_t *)((uint8_t *)inaddr + 0x12);

	sprintf_s(ipAddrStr, 64, "%hd.%hd.%hd.%hd:%hd (%hd)", ip0, ip1, ip2, ip3, port, type);

	return ipAddrStr;
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

const auto ManagedSession_CreateSessionInternal = reinterpret_cast<DWORD(__cdecl *)(int32_t, int32_t)>(0x481550);
DWORD __cdecl ManagedSession_CreateSessionInternalHook(int32_t a1, int32_t a2)
{
	DWORD isOnline = *(DWORD*)a2;
	bool isHost = (*(uint16_t *)(a2 + 284) & 1);

	auto retval = ManagedSession_CreateSessionInternal(a1, a2);

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
		}
		// TODO: give output if StartInfoServer fails
		Patches::Network::StartInfoServer();
		*/
	}
	else
	{
		/*
		Patches::Network::StopInfoServer();
		//Stop the VoIP Server and client
		StopTeamspeakClient();
		StopTeamspeakServer();
		*/
	}

	return retval;
}

// Base class for a class which adds data to player properties.
class PlayerPropertiesExtensionBase
{
public:
	virtual ~PlayerPropertiesExtensionBase() { }

	// Builds extension data for a player.
	virtual void BuildData(int playerIndex, void *out) = 0;

	// Gets the size of the extension data.
	virtual size_t GetDataSize() const = 0;

	// Applies extension data to a player.
	virtual void ApplyData(int playerIndex, Blam::Game::PlayerProperties *properties, const void *data) = 0;

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
	virtual void BuildData(int playerIndex, TData *out) = 0;

	// Applies extension data to a player.
	virtual void ApplyData(int playerIndex, Blam::Game::PlayerProperties *properties, const TData &data) = 0;

	// Serializes the extension data to be sent across the network.
	virtual void Serialize(Blam::Data::BitStream *stream, const TData &data) = 0;

	// Deserializes extension data that was received from the network.
	virtual void Deserialize(Blam::Data::BitStream *stream, TData *out) = 0;

public:
	void BuildData(int playerIndex, void *out) override
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
class PlayerPropertiesExtender : public Utils::Singleton<PlayerPropertiesExtender>
{
public:
	// Adds an extension to the player-properties packet.
	void Add(std::shared_ptr<PlayerPropertiesExtensionBase> extension)
	{
		extensions.push_back(extension);
	}

	// Gets the total size of the player-properties extension data.
	size_t GetTotalSize()
	{
		size_t result = 0;
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
			result += (*it)->GetDataSize();
		return result;
	}

	// Writes all extension data out to a player-properties structure.
	void BuildData(int playerIndex, void *out)
	{
		// Write all of the data structures in order
		uint8_t *ptr = static_cast<uint8_t*>(out);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->BuildData(playerIndex, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	// Applies all extension data in a player-properties structure.
	void ApplyData(int playerIndex, Blam::Game::PlayerProperties *properties, const void *data)
	{
		// Apply all of the data structures in order
		const uint8_t *ptr = static_cast<const uint8_t*>(data);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->ApplyData(playerIndex, properties, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	// Serializes all extension data in a player-properties structure.
	void SerializeData(Blam::Data::BitStream *stream, const void *data)
	{
		// Serialize all of the structures in order
		const uint8_t *ptr = static_cast<const uint8_t*>(data);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->Serialize(stream, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

	// Deserializes all extension data in a player-properties structure.
	void DeserializeData(Blam::Data::BitStream *stream, void *out)
	{
		// Deserialize all of the structures in order
		uint8_t *ptr = static_cast<uint8_t*>(out);
		for (auto it = extensions.begin(); it != extensions.end(); ++it)
		{
			(*it)->Deserialize(stream, ptr);
			ptr += (*it)->GetDataSize();
		}
	}

private:
	std::vector<std::shared_ptr<PlayerPropertiesExtensionBase>> extensions;
};

// Packet size constants
const size_t PlayerPropertiesPacketHeaderSize = 0x18;
const size_t PlayerPropertiesSize = 0x30;
const size_t PlayerPropertiesPacketFooterSize = 0x4;

size_t GetPlayerPropertiesPacketSize()
{
	static size_t size;

	if (size == 0)
	{
		size_t extensionSize = PlayerPropertiesExtender::Instance().GetTotalSize();
		size = PlayerPropertiesPacketHeaderSize + PlayerPropertiesSize + extensionSize + PlayerPropertiesPacketFooterSize;
	}

	return size;
}

// ASCII chars that can't appear in names
const wchar_t DisallowedNameChars[] = { '\'', '\"', '<', '>', '/', '\\' };

void SanitizePlayerName(wchar_t *name)
{
	int i, dest = 0;
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
void __fastcall ApplyPlayerPropertiesExtendedHook(Blam::Network::SessionMembership *thisPtr, void *unused, int playerIndex, uint32_t arg4, uint32_t arg8, uint8_t *data, uint32_t arg10)
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
	PlayerPropertiesExtender::Instance().ApplyData(playerIndex, properties, data + PlayerPropertiesSize);
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
	auto packetSize = GetPlayerPropertiesPacketSize();
	auto extendedSize = packetSize - PlayerPropertiesPacketHeaderSize - PlayerPropertiesPacketFooterSize;
	auto extendedProperties = std::make_unique<uint8_t[]>(extendedSize);
	memcpy(&extendedProperties[0], properties, PlayerPropertiesSize);
	PlayerPropertiesExtender::Instance().BuildData(playerIndex, &extendedProperties[PlayerPropertiesSize]);

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
		typedef void(*InitPacketPtr)(int id, void *packet);
		InitPacketPtr InitPacket = reinterpret_cast<InitPacketPtr>(0x482040);
		InitPacket(thisPtr->AddressIndex, &packet[0]);

		// Set up the header and footer
		*reinterpret_cast<int*>(&packet[0x10]) = arg0;
		*reinterpret_cast<uint32_t*>(&packet[0x14]) = arg4;
		*reinterpret_cast<uint32_t*>(&packet[packetSize - PlayerPropertiesPacketFooterSize]) = argC;

		// Copy the player properties structure in
		memcpy(&packet[PlayerPropertiesPacketHeaderSize], &extendedProperties[0], extendedSize);

		// Send!
		thisPtr->Observer->ObserverChannelSendMessage(thisPtr->Unknown10, channelIndex, 0, 0x1A, packetSize, &packet[0]);
	}
	return true;
}

const auto RegisterPacket = reinterpret_cast<void(__thiscall *)(void *, int32_t, const char *, int32_t, int32_t, int32_t, void *, void *, int32_t, int32_t)>(0x4801B0);

// Changes the size of the player-properties packet to include extension data
void __fastcall RegisterPlayerPropertiesPacketHook(void *thisPtr, void *unused, int packetId, const char *packetName, int arg8, int size1, int size2, void *serializeFunc, void *deserializeFunc, int arg1C, int arg20)
{
	size_t newSize = GetPlayerPropertiesPacketSize();
	RegisterPacket(thisPtr, packetId, packetName, arg8, newSize, newSize, serializeFunc, deserializeFunc, arg1C, arg20);
}

const auto SerializePlayerProperties = reinterpret_cast<void(*)(Blam::Data::BitStream *, uint8_t *, bool)>(0x4433C0);

// Serializes extended player-properties data
void SerializePlayerPropertiesHook(Blam::Data::BitStream *stream, uint8_t *buffer, bool flag)
{
	// Serialize base data
	SerializePlayerProperties(stream, buffer, flag);

	// Serialize extended data
	PlayerPropertiesExtender::Instance().SerializeData(stream, buffer + PlayerPropertiesSize);
}

const auto DeserializePlayerProperties = reinterpret_cast<bool(*)(Blam::Data::BitStream *, uint8_t *, bool)>(0x4432E0);

// Deserializes extended player-properties data
bool DeserializePlayerPropertiesHook(Blam::Data::BitStream *stream, uint8_t *buffer, bool flag)
{
	// Deserialize base data
	bool succeeded = DeserializePlayerProperties(stream, buffer, flag);

	// Deserialize extended data
	if (succeeded)
		PlayerPropertiesExtender::Instance().DeserializeData(stream, buffer + PlayerPropertiesSize);

	return succeeded;
}

const auto Network_Leader_RequestBootMachine = reinterpret_cast<bool(__thiscall *)(void *, void *, int32_t)>(0x45D4A0);

bool __fastcall RequestBootMachineHook(void *thisPtr, void *unused, Blam::Network::PeerInfo *peer, int reason)
{
	auto session = Blam::Network::GetActiveSession();
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

typedef std::function<void(const Blam::Network::NetworkAddress &, uint32_t, uint16_t, uint32_t)> PongCallback;
std::vector<PongCallback> g_PongCallbacks;

void PongReceivedImpl(const Blam::Network::NetworkAddress &from, const Blam::Network::PongPacket &pong, uint32_t latency)
{
	for (auto &&callback : g_PongCallbacks)
		callback(from, pong.Timestamp, pong.ID, latency);
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

typedef std::function<void(Blam::Network::LifeCycleState)> LifeCycleStateChangedCallback;
std::vector<LifeCycleStateChangedCallback> g_LifeCycleStateChangedCallbacks;

void LifeCycleStateChangedImpl(Blam::Network::LifeCycleState p_NewState)
{
	for (auto &&s_Callback : g_LifeCycleStateChangedCallbacks)
		s_Callback(p_NewState);
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
	// Convert the IP to a string
	struct in_addr s_InAddr;
	s_InAddr.S_un.S_addr = p_Address.ToInAddr();
	char ipStr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &s_InAddr, ipStr, sizeof(ipStr)))
	{
		/* TODO:
		// Check if the IP is in the ban list
		auto banList = Server::LoadDefaultBanList();
		if (banList.ContainsIp(ipStr))
		{
		// Send a join refusal
		typedef void(__thiscall *Network_session_acknowledge_join_requestFunc)(Blam::Network::Session *thisPtr, const Blam::Network::NetworkAddress &address, int reason);
		auto Network_session_acknowledge_join_request = reinterpret_cast<Network_session_acknowledge_join_requestFunc>(0x45A230);
		Network_session_acknowledge_join_request(thisPtr, address, 0); // TODO: Use a special code for bans and hook the join refusal handler so we can display a message to the player
		Utils::Logger::Instance().Log(Utils::LogTypes::Network, Utils::LogLevel::Info, "Refused join request from banned IP %s", ipStr);
		return true;
		}
		*/
	}

	// Continue the join process
	typedef bool(__thiscall *Network_session_handle_join_requestFunc)(Blam::Network::Session *thisPtr, const Blam::Network::NetworkAddress &address, void *request);
	auto Network_session_handle_join_request = reinterpret_cast<Network_session_handle_join_requestFunc>(0x4DA410);
	return Network_session_handle_join_request(p_This, p_Address, p_Request);
}

const auto Network_State_EndGame_WriteStatsEnter = reinterpret_cast<char(__thiscall *)(void *, int32_t, int32_t, int32_t)>(0x492B50);

char __fastcall Network_State_EndGame_WriteStatsEnter_Hook(void* thisPtr, int unused, int a2, int a3, int a4)
{
	// There used to be a stats upload here, which is why this hook does nothing
	return Network_State_EndGame_WriteStatsEnter(thisPtr, a2, a3, a4);
}

const auto Network_State_LeavingEnter = reinterpret_cast<int8_t(__thiscall *)(void *, int32_t, int32_t, int32_t)>(0x4933E0);

int8_t __fastcall Network_State_LeavingEnter_Hook(void* thisPtr, int32_t unused, int32_t a2, int32_t a3, int32_t a4)
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
	int maxPlayers = Modules::ModuleServer::Instance().VarServerMaxPlayers->ValueInt;
	return Utils::Clamp(maxPlayers, 1, 16);*/

	return 16;
}

bool __fastcall Network_GetEndpoint_Hook(char *thisPtr, void *unused)
{
	char *socket = thisPtr + 12;
	uint32_t port = 11775;/* TODO: Modules::ModuleServer::Instance().VarServerGamePort->ValueInt; */
	bool success = false;

	//bool __cdecl Network_c_network_link::create_endpoint(int a1, __int16 GamePort, char a3, _DWORD *a4)
	typedef bool(__cdecl *Network_link_create_endpointFunc)(int a1, __int16 GamePort, char a3, void *a4);
	Network_link_create_endpointFunc Network_link_create_endpoint = reinterpret_cast<Network_link_create_endpointFunc>(0x43B6F0);

	//LPVOID __cdecl sub_43FED0(SOCKET socket)
	typedef LPVOID(__cdecl *sub_43FED0Func)(SOCKET socket);
	sub_43FED0Func sub_43FED0 = reinterpret_cast<sub_43FED0Func>(0x43FED0);

	while (true)
	{
		*reinterpret_cast<uint32_t *>(0x1860454) = port;
		success = Network_link_create_endpoint(0, (short)port, 1, socket);

		if (success)
			break;

		if (*socket)
		{
			sub_43FED0(*socket);
			*socket = 0;
		}

		if (++port - 11775 /* TODO: Modules::ModuleServer::Instance().VarServerGamePort->ValueInt */ >= 1000)
		{
			*reinterpret_cast<uint32_t *>(0x1860454) = 11775; /* TODO: Modules::ModuleServer::Instance().VarServerGamePort->ValueInt; */
			return success;
		}
	}
	return success;
}

char __fastcall Network_Session_JoinRemoteSession_Hook(void* thisPtr, int unused, char a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, void *a12, int a13, int a14)
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

	typedef char(__fastcall *Network_session_join_remote_sessionFunc)(void* thisPtr, int unused, char a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, void *a12, int a13, int a14);
	const auto Network_Session_JoinRemoteSession = reinterpret_cast<Network_session_join_remote_sessionFunc>(0x45D1E0);
	return Network_Session_JoinRemoteSession(thisPtr, unused, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);;
}

// This is only hooked on error states
int __fastcall Network_Session_InitiateLeaveProtocol_Hook(void* thisPtr, int unused, char forceClose)
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

	typedef int(__fastcall *Network_session_initiate_leave_protocolFunc)(void* thisPtr, int unused, char forceClose);
	const auto Network_Session_InitiateLeaveProtocol = reinterpret_cast<Network_session_initiate_leave_protocolFunc>(0x45CB80);
	return Network_Session_InitiateLeaveProtocol(thisPtr, unused, forceClose);
}

// Hooked on the initial update that fires on server connect
int __fastcall Network_Session_ParametersClear_Hook(void* thisPtr, int unused)
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

	typedef int(__fastcall *Network_session_parameters_clearFunc)(void* thisPtr, int unused);
	const auto Network_Session_ParametersClear = reinterpret_cast<Network_session_parameters_clearFunc>(0x486580);
	return Network_Session_ParametersClear(thisPtr, unused);
}
bool Engine::Init()
{
	//Disable Windows DPI scaling
	SetProcessDPIAware();

	// Enable write to all executable memory
	UnprotectMemory();

	if (!Blam::Cache::StringIDCache::Instance.Load("maps\\string_ids.dat"))
	{
		WriteLog("Failed to load 'maps\\string_ids.dat'!");
		return false;
	}
	
	// English patch
	Util::PatchAddress(0x2333FD, "\x00", 1);

	// Game window creation callbacks
	Util::ApplyHook(0x622057, CreateGameWindowHook, HookFlags::IsCall);

	// Hook window title sprintf to replace the dest buf with our string
	Util::ApplyHook(0x2EB84, WindowTitleSprintfHook, HookFlags::IsCall);

	// Runs when the game's resolution is changed
	Util::ApplyHook(0x621303, ResolutionChangeHook, HookFlags::IsCall);

	//Fix aspect ratio not matching resolution
	Util::ApplyHook(0x6648C9, AspectRatioHook, HookFlags::IsCall);
	Util::ApplyHook(0x216487, AspectRatioHook, HookFlags::IsCall);

	//Disable converting the game's resolution to 16:9
	Util::PatchAddress(0x62217D, "\x90\x90", 2);
	Util::PatchAddress(0x622183, "\x90\x90\x90\x90\x90\x90", 6);

	//Allow the user to select any resolution that Windows supports in the settings screen.
	Util::PatchAddress(0x10BF1B, "\x90\x90", 2);
	Util::PatchAddress(0x10BF21, "\x90\x90\x90\x90\x90\x90", 6);

	// Prevent FOV from being overridden when the game loads
	Util::PatchAddress(0x25FA79, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);
	Util::PatchAddress(0x25FA86, "\x90\x90\x90\x90\x90", 5);
	Util::ApplyHook(0x10CA02, FovHook);

	// Force descoping for all weapons
	Util::PatchAddress(0x73F1E6, "\x00", 1);

	// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
	Util::PatchAddress(0x100DA75, "\x02", 1);

	// Increase max virtual audio channels from 64 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
	Util::ApplyHook(0x4E9C, FmodSystemInitHook);
	Util::ApplyHook(0x4EC0, FmodSystemInitHook2);

	// Increase software channels from 192 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
	Util::PatchAddress(0x4DF9, "\x00\x00\x01\x00", 4);

	// Enable Tag Editing
	Util::PatchAddress(0x101A5B, "\xEB", 1);
	Util::PatchAddress(0x102874, "\x90\x90", 2);
	Util::PatchAddress(0x1030AA, "\x90\x90", 2);

	// No --account args patch
	Util::PatchAddress(0x43731A, "\xEB\x0E", 2);
	Util::PatchAddress(0x4373AD, "\xEB\x03", 2);

	// Remove preferences.dat hash check
	Util::PatchAddress(0x10C99A, "\x90\x90\x90\x90\x90\x90", 6);

	// Patch to allow spawning AI through effects
	Util::PatchAddress(0x1033321, "\x90\x90", 2);

	// Fix random colored lighting
	Util::PatchAddress(0x14F2FFC, "\x00\x00\x00\x00", 4);

	// Used to call Patches::ApplyAfterTagsLoaded when tags have loaded
	Util::ApplyHook(0x1030EA, TagsLoadedHook);

	// Prevent game variant weapons from being overridden
	Util::PatchAddress(0x1A315F, "\xEB", 1);
	Util::PatchAddress(0x1A31A4, "\xEB", 1);
	Util::ApplyHook(0x1A3267, GrenadeLoadoutHook);

	// Hook game ticks
	Util::ApplyHook(0x105ABA, GameTickHook, HookFlags::IsCall);
	Util::ApplyHook(0x1063E6, GameTickHook, HookFlags::IsCall);

	// Run callbacks on engine shutdown
	Util::ApplyHook(0x2EBD7, ShutdownHook, HookFlags::IsCall);

	// Map loading
	Util::ApplyHook(0x10FC2C, LoadMapHook, HookFlags::IsCall);
	Util::ApplyHook(0x1671BE, LoadMapHook, HookFlags::IsCall);
	Util::ApplyHook(0x167B4F, LoadMapHook, HookFlags::IsCall);

	// Remove aim assist for keyboard/mouse input
	Util::ApplyHook(0x18AA17, AimAssistHook);
	Util::ApplyHook(0x18ABAB, DualAimAssistHook, HookFlags::IsCall);

	// Rewire $hq.MatchmakingLeaveQueue() to end the game
	Util::ApplyHook(0x3B6826, EndGameHook, HookFlags::IsCall);
	Util::PatchAddress(0x3B682B, "\x90", 1);

	// Rewire $hf2pEngine.PerformLogin() to show the pause menu
	Util::ApplyHook(0x234756, &ShowHalo3PauseMenuHook, HookFlags::IsCall);
	Util::PatchAddress(0x23475B, "\x90", 1);

	// Allows you to press B to close the H3 pause menu
	// TODO: find out what the byte that's being checked does, we're just patching out the check here but maybe it's important
	Util::PatchAddress(0x6E05F3, "\x90\x90", 2);

	// Fix "Network" setting in lobbies (change broken 0x100B7 menuID to 0x100B6)
	Util::PatchAddress(0x6C34B0, "\xB6", 1);

	// Fix gamepad option in settings (todo: find out why it doesn't detect gamepads
	// and find a way to at least enable pressing ESC when gamepad is enabled)
	Util::PatchAddress(0x20D7F2, "\x90\x90", 2);

	// Fix menu update code to include missing mainmenu code
	Util::ApplyHook(0x6DFB73, MenuUpdateHook, HookFlags::IsCall);

	// Sorta hacky way of getting game options screen to show when you press X on lobby
	// TODO: find real way of showing the [X] Edit Game Options text, that might enable it to work without patching
	Util::ApplyHook(0x721B8A, LobbyMenuButtonHandlerHook, HookFlags::IsJmpIfEqual);

	// Remove Xbox Live from the network menu
	Util::PatchAddress(0x723D85, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 23);
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723DA1) = 0;
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723DB8) = 1;
	Util::PatchAddress(0x723DFF, "\x90\x90\x90", 3);
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723E07) = 0;
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723E1C) = 0;

	// Localized string override hook
	Util::ApplyHook(0x11E040, LocalizedStringHook);

	// Remove "BUILT IN" text when choosing map/game variants by feeding the UI_SetVisiblityOfElement func a nonexistant string ID for the element (0x202E8 instead of 0x102E8)
	// TODO: find way to fix this text instead of removing it, since the 0x102E8 element (subitem_edit) is used for other things like editing/viewing map variant metadata
	Util::PatchAddress(0x705D6F, "\x02", 1);

	// Hook the call to create a lobby from the main menu so that we
	// can show the server browser if matchmaking is selected
	Util::ApplyHook(0x6E79A7, MainMenuCreateLobbyHook, HookFlags::IsCall);

	// Enable H3UI scaling
	Util::PatchAddress(0x61FAD1, "\x90\x90", 2);

	// Change the way that Forge handles dpad up so that it doesn't mess with key repeat
	// Comparing the action tick count to 1 instead of using the "handled" flag does roughly the same thing and lets the menu UI read the key too
	Util::PatchAddress(0x19F17F, "\x75", 1);
	Util::PatchAddress(0x19F198, "\x90\x90\x90\x90", 4);

	// Fix network debug strings having (null) instead of an IP address
	Util::ApplyHook(0x3F6F0, GetIPStringFromInAddrHook);

	// Fix for XnAddrToInAddr to try checking syslink-menu data for XnAddr->InAddr mapping before consulting XNet
	Util::ApplyHook(0x30B6C, XnAddrToInAddrHook, HookFlags::IsCall);
	Util::ApplyHook(0x30F51, InAddrToXnAddrHook, HookFlags::IsCall);

	// Hook call to Network_managed_session_create_session_internal so we can detect when an online game is started
	Util::ApplyHook(0x82AAC, ManagedSession_CreateSessionInternalHook, HookFlags::IsCall);

	// Patch version subs to return version of this DLL, to make people with older DLLs incompatible
	uint32_t verNum = ANVIL_BUILD;
	*reinterpret_cast<uint32_t *>((uint8_t *)GetModuleBase() + 0x101421) = verNum;
	*reinterpret_cast<uint32_t *>((uint8_t *)GetModuleBase() + 0x10143A) = verNum;

	// Player-properties packet hooks
	Util::ApplyHook(0x5DD20, PeerRequestPlayerDesiredPropertiesUpdateHook);
	Util::ApplyHook(0xDAF4F, ApplyPlayerPropertiesExtendedHook, HookFlags::IsCall);
	Util::ApplyHook(0xDFF7E, RegisterPlayerPropertiesPacketHook, HookFlags::IsCall);
	Util::ApplyHook(0xDFD53, SerializePlayerPropertiesHook, HookFlags::IsCall);
	Util::ApplyHook(0xDE178, DeserializePlayerPropertiesHook, HookFlags::IsCall);

	// Hook leader_request_boot_machine so we can do some extra things if the boot succeeded
	Util::ApplyHook(0x37E17, RequestBootMachineHook, HookFlags::IsCall);

	// Pong hook
	Util::ApplyHook(0x9D9DB, PongReceivedHook);

	// Lifecycle state change hook
	Util::ApplyHook(0x8E527, LifeCycleStateChangedHook, HookFlags::IsCall);
	Util::ApplyHook(0x8E10F, LifeCycleStateChangedHook, HookFlags::IsCall);

	// Hook the join request handler to check the user's IP address against the ban list
	Util::ApplyHook(0x9D0F7, Network_Session_HandleJoinRequest_Hook, HookFlags::IsCall);

	// Hook c_life_cycle_state_handler_end_game_write_stats's vftable ::entry method
	DWORD s_Temp1;
	DWORD s_Temp2;
	auto s_Address = reinterpret_cast<uint32_t *>(0x16183A0);
	if (!VirtualProtect(s_Address, 4, PAGE_READWRITE, &s_Temp1))
	{
		std::stringstream ss;
		ss << "Failed to set protection on memory address " << std::hex << (void*)s_Address;
		OutputDebugString(ss.str().c_str());
	}
	else
	{
		*s_Address = (uint32_t)&Network_State_EndGame_WriteStatsEnter_Hook;
		VirtualProtect(s_Address, 4, s_Temp1, &s_Temp2);
	}

	// Hook c_life_cycle_state_handler_leaving's vftable ::entry method
	s_Address = reinterpret_cast<uint32_t *>(0x16183BC);
	if (!VirtualProtect(s_Address, 4, PAGE_READWRITE, &s_Temp1))
	{
		std::stringstream ss;
		ss << "Failed to set protection on memory address " << std::hex << (void*)s_Address;
		OutputDebugString(ss.str().c_str());
	}
	else
	{
		*s_Address = (uint32_t)&Network_State_LeavingEnter_Hook;
		VirtualProtect(s_Address, 4, s_Temp1, &s_Temp2);
	}

	// Set the max player count to Server.MaxPlayers when hosting a lobby
	Util::ApplyHook(0x67FA0D, Network_GetMaxPlayers_Hook, HookFlags::IsCall);

	Util::ApplyHook(0x3BAFB, Network_GetEndpoint_Hook, HookFlags::IsCall);

	Util::ApplyHook(0x7F5B9, Network_Session_JoinRemoteSession_Hook, HookFlags::IsCall);

	// Hook Network_session_initiate_leave_protocol in Network_session_idle_peer_joining's error states
	// "peer join timed out waiting for secure connection to become established"
	Util::ApplyHook(0x9BFCA, Network_Session_InitiateLeaveProtocol_Hook, HookFlags::IsCall);
	// "peer join timed out waiting for initial updates"
	Util::ApplyHook(0x9C0BE, Network_Session_InitiateLeaveProtocol_Hook, HookFlags::IsCall);

	// "received initial update, clearing"
	Util::ApplyHook(0x899AF, Network_Session_ParametersClear_Hook, HookFlags::IsCall);

	return true;
}