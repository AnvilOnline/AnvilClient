#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include "BuildInfo.hpp"
#include "Globals.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	typedef std::function<void(HWND window)> CreateWindowCallback;
	std::vector<CreateWindowCallback> g_CreateWindowCallbacks;

	const auto UI_CreateGameWindow = reinterpret_cast<HWND(*)()>(0xA223F0);
	HWND CreateGameWindowHook()
	{
		auto s_HWND = UI_CreateGameWindow();
		if (!s_HWND)
			return nullptr;

		for (auto &s_Callback : g_CreateWindowCallbacks)
			s_Callback(s_HWND);

		return s_HWND;
	}

	void WindowTitleSprintfHook(char *p_DestBuf, char *p_Format, char *p_Version)
	{
		strcpy_s(p_DestBuf, 0x40, AnvilCommon::g_BuildInfo.c_str());
	}

	//
	// TODO: Find a better place for the dialog variables below...
	//

	bool g_DialogShow;
	uint32_t g_DialogStringId;
	int32_t g_DialogArg1;
	int32_t g_DialogFlags;
	uint32_t g_DialogParentStringId;
	void *g_UiData = 0;

	int32_t ShowHalo3PauseMenuHook(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
	{
		g_UiData = nullptr;
		g_DialogStringId = 0x10084;
		g_DialogArg1 = 0;
		g_DialogFlags = 4;
		g_DialogParentStringId = 0x1000C;
		g_DialogShow = true;

		return 1;
	}

	void __fastcall MenuUpdateHook(void *a1, int32_t unused, int32_t menuIdToLoad)
	{
		/*auto& dorito = ElDorito::Instance();
		if (menuIdToLoad == 0x10083)
		dorito.OnMainMenuShown();*/

		bool shouldUpdate = *(uint32_t *)((uint8_t*)a1 + 0x10) >= 0x1E;
		int32_t uiData0x18Value = 1;
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

	bool LocalizedStringImpl(int32_t tagIndex, int32_t stringId, wchar_t *outputBuffer)
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

		fallback: // Execute replaced code and jump back to original function
			sub esp, 0x800
			mov edx, 0x51E049
			jmp edx
		}
	}

	const auto UI_CreateLobby = reinterpret_cast<bool(*)(int32_t)>(0xA7EE70);
	bool MainMenuCreateLobbyHook(int32_t lobbyType)
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

	bool Engine::ApplyPatches_UserInterface()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Game window creation callbacks
		return Hook(0x622057, CreateGameWindowHook, HookFlags::IsCall).Apply()
			// Hook window title sprintf to replace the dest buf with our string
			&& Hook(0x2EB84, WindowTitleSprintfHook, HookFlags::IsCall).Apply()
			// Rewire $hf2pEngine.PerformLogin() to show the pause menu
			&& Hook(0x234756, ShowHalo3PauseMenuHook, HookFlags::IsCall).Apply()
			&& Patch::NopFill(0x23475B, 1)
			// Allows you to press B to close the H3 pause menu
			// TODO: find out what the byte that's being checked does, we're just patching out the check here but maybe it's important
			&& Patch::NopFill(0x6E05F3, 2)
			// Fix "Network" setting in lobbies (change broken 0x100B7 menuID to 0x100B6)
			&& Patch(0x6C34B0, 0xB6).Apply()
			// Fix gamepad option in settings (todo: find out why it doesn't detect gamepads
			// and find a way to at least enable pressing ESC when gamepad is enabled)
			&& Patch::NopFill(0x20D7F2, 2)
			// Fix menu update code to include missing mainmenu code
			&& Hook(0x6DFB73, MenuUpdateHook, HookFlags::IsCall).Apply()
			// Sorta hacky way of getting game options screen to show when you press X on lobby
			// TODO: find real way of showing the [X] Edit Game Options text, that might enable it to work without patching
			&& Hook(0x721B8A, LobbyMenuButtonHandlerHook, HookFlags::IsJmpIfEqual).Apply()
			// Remove Xbox Live from the network menu
			&& Patch::NopFill(0x723D85, 23)
			&& Patch(0x723DA1, 0x00).Apply()
			&& Patch(0x723DB8, 0x01).Apply()
			&& Patch::NopFill(0x723DFF, 3)
			&& Patch(0x723E07, 0x00).Apply()
			&& Patch(0x723E1C, 0x00).Apply()
			// Localized string override hook
			&& Hook(0x11E040, LocalizedStringHook).Apply()
			// Remove "BUILT IN" text when choosing map/game variants by feeding the UI_SetVisiblityOfElement func a nonexistant string ID for the element (0x202E8 instead of 0x102E8)
			// TODO: find way to fix this text instead of removing it, since the 0x102E8 element (subitem_edit) is used for other things like editing/viewing map variant metadata
			&& Patch(0x705D6F, 0x02).Apply()
			// Hook the call to create a lobby from the main menu so that we
			// can show the server browser if matchmaking is selected
			&& Hook(0x6E79A7, MainMenuCreateLobbyHook, HookFlags::IsCall).Apply()
			// Enable H3UI scaling
			&& Patch::NopFill(0x61FAD1, 2);
	}
}