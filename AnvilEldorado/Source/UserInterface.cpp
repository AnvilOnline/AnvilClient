#include <vector>

#include "Utils\Logger.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"

#include "Blam\Cache\StringIDCache.hpp"
#include "Blam\Input\InputTypes.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\UserInterface\ChudDefinition.hpp"
#include "Blam\Tags\UserInterface\ChudGlobalsDefinition.hpp"

#include "BuildInfo.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "UserInterface.hpp"

namespace AnvilEldorado
{
	HWND CreateGameWindowHook()
	{
		return UserInterface::Instance()->CreateGameWindow();
	}

	void WindowTitleSprintfHook(char *p_DestBuf, char *p_Format, char *p_Version)
	{
		strcpy_s(p_DestBuf, 0x40, AnvilCommon::g_BuildInfo.c_str());
	}

	int32_t ShowHalo3PauseMenuHook(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
	{
		auto *s_UserInterface = UserInterface::Instance();

		return (int32_t)(s_UserInterface->ShowDialog(0x10084, 0, 4, 0x1000C) != nullptr);
	}

	const auto UI_MenuUpdate = reinterpret_cast<void(__thiscall *)(void *, Blam::Text::StringID)>(0xADF6E0);

	void __fastcall MenuUpdateHook(void *a1, int32_t unused, Blam::Text::StringID p_MenuName)
	{
		auto s_ShouldUpdate = *(uint32_t *)((uint8_t*)a1 + 0x10) >= 0x1E;

		UI_MenuUpdate(a1, p_MenuName);

		if (s_ShouldUpdate)
		{
			void *s_UIData = UserInterface::Instance()->ShowDialog(p_MenuName, 0xFF, 4, 0x1000D);

			if (s_UIData != nullptr)
				*(uint32_t*)((char*)s_UIData + 0x18) = 1;
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

	bool LocalizedStringImpl(int32_t p_TagIndex, Blam::Text::StringID p_String, wchar_t *p_Output)
	{
		if (p_String == "start_new_campaign")
		{
			// Get the version string, convert it to uppercase UTF-16, and return it
			auto s_BuildInfo = std::string("  ANVIL: ONLINE  ") + std::string("DEV BUILD ") + std::to_string(ANVIL_BUILD);
			auto s_WideBuildInfo = std::wstring(s_BuildInfo.begin(), s_BuildInfo.end());
			swprintf(p_Output, MaxStringLength, s_WideBuildInfo.c_str());
			
			return true;
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

	const auto UI_CreateLobby = reinterpret_cast<bool(*)(GameLobbyType)>(0xA7EE70);
	bool MainMenuCreateLobbyHook(GameLobbyType p_LobbyType)
	{
		// TODO: Intercept creation of certain lobby types if need be...

		return UI_CreateLobby(p_LobbyType);
	}

	const auto UI_Forge_ButtonPressHandler = reinterpret_cast<bool(__thiscall *)(void *, void *)>(0xAE2180);

	bool __fastcall UI_Forge_ButtonPressHandler_Hook(void *p_Arg1, int32_t, uint8_t *p_ControllerData)
	{
		// TODO: Map out controller data structure

		auto *s_ButtonCode = &*(uint32_t*)(p_ControllerData + 0x1C);

		switch (*s_ButtonCode)
		{
			case Blam::Input::eUiButtonCodeLeft:
			case Blam::Input::eUiButtonCodeDpadLeft:
			{
				*s_ButtonCode = Blam::Input::eUiButtonCodeLB;
				break;
			}

			case Blam::Input::eUiButtonCodeRight:
			case Blam::Input::eUiButtonCodeDpadRight:
			{
				*s_ButtonCode = Blam::Input::eUiButtonCodeRB;
				break;
			}
		}

		return UI_Forge_ButtonPressHandler(p_Arg1, p_ControllerData);
	}

	bool UserInterface::Init()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Hook UI vftable's forge menu button handler, so arrow keys can act as bumpers
			// added side effect: analog stick left/right can also navigate through menus
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
			&& Patch::NopFill(0x61FAD1, 2)
			// Hook UI vftable's forge menu button handler, so arrow keys can act as bumpers
			// added side effect: analog stick left/right can also navigate through menus
			// TODO: Move to input class?
			&& Hook(0x129EFD8, UI_Forge_ButtonPressHandler_Hook, HookFlags::IsVirtual).Apply();
	}

	const auto UI_CreateGameWindow = reinterpret_cast<HWND(*)()>(0xA223F0);
	HWND UserInterface::CreateGameWindow()
	{
		auto s_HWND = UI_CreateGameWindow();

		if (!s_HWND)
			return nullptr;

		for (auto &s_Callback : m_CreateWindowCallbacks)
			s_Callback(s_HWND);

		return s_HWND;
	}

	bool UserInterface::ShouldScaleInterface() const
	{
		return m_ScaleInterface;
	}

	void UserInterface::SetScaleInterface(const bool p_ScaleInterface)
	{
		m_ScaleInterface = p_ScaleInterface;
	}

	void UserInterface::ApplyResolution()
	{
		using Blam::Tags::TagInstance;
		using Blam::Tags::UserInterface::ChudGlobalsDefinition;
		using Blam::Tags::UserInterface::ChudDefinition;

		auto *s_GameResolution = reinterpret_cast<int *>(0x19106C0);
		auto *s_ChudGlobalsDefinition = TagInstance(0x01BD).GetDefinition<ChudGlobalsDefinition>();
		auto *s_ChudDefinition = TagInstance(0x0C1E).GetDefinition<ChudDefinition>();

		// Store initial HUD resolution values the first time the resolution is changed.
		if (m_FirstResolutionChange)
		{
			m_HudResolutionWidth = s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionWidth;
			m_HudResolutionHeight = s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionHeight;
			m_HudResolutionScaleX = s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].HorizontalScale;
			m_HudResolutionScaleY = s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].VerticalScale;
			m_HudMotionSensorOffsetX = s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].MotionSensorOffsetX;

			// Store bottom visor offset
			for (auto &s_HudWidget : s_ChudDefinition->HudWidgets)
			{
				if (s_HudWidget.Name == "in_helmet_bottom_new")
				{
					m_HudBottomVisorOffsetY = s_HudWidget.PlacementData[0].OffsetY;
					break;
				}
			}

			m_FirstResolutionChange = false;
		}

		// Make UI match it's original width of 1920 pixels on non-widescreen monitors.
		// Fixes the visor getting cut off.
		s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionWidth = m_HudResolutionWidth;

		// H3UI Resolution
		auto *s_InterfaceResolution = reinterpret_cast<int *>(0x19106C8);

		if ((s_GameResolution[0] / 16 > s_GameResolution[1] / 9))
		{
			// On aspect ratios with a greater width than 16:9 center the UI on the screen
			s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionHeight = m_HudResolutionHeight;
			s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].HorizontalScale = (s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionWidth / (float)s_GameResolution[0]) * m_HudResolutionScaleX;
			s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].VerticalScale = (s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionHeight / (float)s_GameResolution[1]) * m_HudResolutionScaleY;

			s_InterfaceResolution[0] = (int)(((float)s_GameResolution[0] / (float)s_GameResolution[1]) * 640);
			s_InterfaceResolution[1] = 640;
		}
		else
		{
			s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionHeight = (int)(((float)s_GameResolution[1] / (float)s_GameResolution[0]) * s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionWidth);
			s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].HorizontalScale = m_HudResolutionScaleX;
			s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].VerticalScale = m_HudResolutionScaleY;

			s_InterfaceResolution[0] = 1152;//1152 x 640 resolution
			s_InterfaceResolution[1] = (int)(((float)s_GameResolution[1] / (float)s_GameResolution[0]) * 1152);
		}

		// Adjust motion sensor blip to match the UI resolution
		s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].MotionSensorOffsetX = m_HudMotionSensorOffsetX;
		s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].MotionSensorOffsetY = (float)(s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionHeight - (s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].MotionSensorRadius - s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].MotionSensorScale));

		// Search for the visor bottom and fix it if found
		for (auto &s_Widget : s_ChudDefinition->HudWidgets)
		{
			if (s_Widget.Name == "in_helmet_bottom_new")
			{
				s_Widget.PlacementData[0].OffsetY = (((float)s_ChudGlobalsDefinition->HudGlobals[0].HudAttributes[0].ResolutionHeight - m_HudResolutionHeight) / 2) + m_HudBottomVisorOffsetY;
				break;
			}
		}
	}

	const auto UI_Alloc = reinterpret_cast<void *(__cdecl *)(int32_t)>(0xAB4ED0);
	const auto UI_OpenDialogById = reinterpret_cast<void *(__thiscall *)(void *, Blam::Text::StringID, int32_t, int32_t, Blam::Text::StringID)>(0xA92780);
	const auto UI_PostMessage = reinterpret_cast<int(*)(void *)>(0xA93450);

	void *UserInterface::ShowDialog(const Blam::Text::StringID &p_DialogID, const int32_t p_Arg1, const int32_t p_Flags, const Blam::Text::StringID &p_ParentID)
	{
		WriteLog("Showing ui dialog '%s'...", Blam::Cache::StringIDCache::Instance()->GetString(p_DialogID));

		auto *s_UIData = UI_Alloc(0x40);

		if (!s_UIData)
			return nullptr;

		UI_OpenDialogById(s_UIData, p_DialogID, p_Arg1, p_Flags, p_ParentID);
		UI_PostMessage(s_UIData);

		return s_UIData;
	}
}