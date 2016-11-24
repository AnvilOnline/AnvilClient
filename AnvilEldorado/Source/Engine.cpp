#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "Globals.hpp"
#include "Utils\Logger.hpp"
#include "Utils\Util.hpp"
#include "Blam\Cache\StringIDCache.hpp"
#include "Blam\Data\DatumIndex.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Network\Network.hpp"
#include "Blam\Tags\TagInstance.hpp"
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

void WindowTitleSprintfHook(char *destBuf, char *format, char *version)
{
	std::string windowTitle = "ANVIL: ONLINE - DEVELOPMENT BUILD";
	strcpy_s(destBuf, 0x40, windowTitle.c_str());
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
	//
	// TODO: Call TagsLoaded function here...
	//
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
			swprintf(outputBuffer, MaxStringLength, L"  ANVIL: ONLINE  DEV BUILD");
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

	fallback:
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
	Util::PatchAddressInFile(0x62217D, "\x90\x90", 2);
	Util::PatchAddressInFile(0x622183, "\x90\x90\x90\x90\x90\x90", 6);

	//Allow the user to select any resolution that Windows supports in the settings screen.
	Util::PatchAddressInFile(0x10BF1B, "\x90\x90", 2);
	Util::PatchAddressInFile(0x10BF21, "\x90\x90\x90\x90\x90\x90", 6);

	// Prevent FOV from being overridden when the game loads
	Util::PatchAddressInFile(0x25FA79, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);
	Util::PatchAddressInFile(0x25FA86, "\x90\x90\x90\x90\x90", 5);
	Util::ApplyHook(0x10CA02, FovHook);

	// Force descoping for all weapons
	Util::PatchAddressInFile(0x73F1E6, "\x00", 1);

	// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
	Util::PatchAddressInFile(0x100DA75, "\x02", 1);

	// Increase max virtual audio channels from 64 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
	Util::ApplyHook(0x4E9C, FmodSystemInitHook);
	Util::ApplyHook(0x4EC0, FmodSystemInitHook2);

	// Increase software channels from 192 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
	Util::PatchAddressInFile(0x4DF9, "\x00\x00\x01\x00", 4);

	// Enable Tag Editing
	Util::PatchAddressInFile(0x101A5B, "\xEB", 1);
	Util::PatchAddressInFile(0x102874, "\x90\x90", 2);
	Util::PatchAddressInFile(0x1030AA, "\x90\x90", 2);

	// No --account args patch
	Util::PatchAddressInFile(0x43731A, "\xEB\x0E", 2);
	Util::PatchAddressInFile(0x4373AD, "\xEB\x03", 2);

	// Remove preferences.dat hash check
	Util::PatchAddressInFile(0x10C99A, "\x90\x90\x90\x90\x90\x90", 6);

	// Patch to allow spawning AI through effects
	Util::PatchAddressInFile(0x1033321, "\x90\x90", 2);

	// Fix random colored lighting
	Util::PatchAddressInFile(0x14F2FFC, "\x00\x00\x00\x00", 4);

	// Used to call Patches::ApplyAfterTagsLoaded when tags have loaded
	Util::ApplyHook(0x1030EA, TagsLoadedHook);

	// Prevent game variant weapons from being overridden
	Util::PatchAddressInFile(0x1A315F, "\xEB", 1);
	Util::PatchAddressInFile(0x1A31A4, "\xEB", 1);
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
	Util::PatchAddressInFile(0x3B682B, "\x90", 1);

	// Rewire $hf2pEngine.PerformLogin() to show the pause menu
	Util::ApplyHook(0x234756, &ShowHalo3PauseMenuHook, HookFlags::IsCall);
	Util::PatchAddressInFile(0x23475B, "\x90", 1);

	// Allows you to press B to close the H3 pause menu
	// TODO: find out what the byte that's being checked does, we're just patching out the check here but maybe it's important
	Util::PatchAddressInFile(0x6E05F3, "\x90\x90", 2);

	// Fix "Network" setting in lobbies (change broken 0x100B7 menuID to 0x100B6)
	Util::PatchAddressInFile(0x6C34B0, "\xB6", 1);

	// Fix gamepad option in settings (todo: find out why it doesn't detect gamepads
	// and find a way to at least enable pressing ESC when gamepad is enabled)
	Util::PatchAddressInFile(0x20D7F2, "\x90\x90", 2);

	// Fix menu update code to include missing mainmenu code
	Util::ApplyHook(0x6DFB73, MenuUpdateHook, HookFlags::IsCall);

	// Sorta hacky way of getting game options screen to show when you press X on lobby
	// TODO: find real way of showing the [X] Edit Game Options text, that might enable it to work without patching
	Util::ApplyHook(0x721B8A, LobbyMenuButtonHandlerHook, HookFlags::IsJmpIfEqual);

	// Remove Xbox Live from the network menu
	Util::PatchAddressInFile(0x723D85, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 23);
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723DA1) = 0;
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723DB8) = 1;
	Util::PatchAddressInFile(0x723DFF, "\x90\x90\x90", 3);
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723E07) = 0;
	*reinterpret_cast<uint8_t *>((uint8_t *)GetModuleBase() + 0x723E1C) = 0;

	// Localized string override hook
	Util::ApplyHook(0x11E040, LocalizedStringHook);

	// Remove "BUILT IN" text when choosing map/game variants by feeding the UI_SetVisiblityOfElement func a nonexistant string ID for the element (0x202E8 instead of 0x102E8)
	// TODO: find way to fix this text instead of removing it, since the 0x102E8 element (subitem_edit) is used for other things like editing/viewing map variant metadata
	Util::PatchAddressInFile(0x705D6F, "\x02", 1);

	// Hook the call to create a lobby from the main menu so that we
	// can show the server browser if matchmaking is selected
	Util::ApplyHook(0x6E79A7, MainMenuCreateLobbyHook, HookFlags::IsCall);

	// Enable H3UI scaling
	Util::PatchAddressInFile(0x61FAD1, "\x90\x90", 2);

	// Change the way that Forge handles dpad up so that it doesn't mess with key repeat
	// Comparing the action tick count to 1 instead of using the "handled" flag does roughly the same thing and lets the menu UI read the key too
	Util::PatchAddressInFile(0x19F17F, "\x75", 1);
	Util::PatchAddressInFile(0x19F198, "\x90\x90\x90\x90", 4);

	return true;
}