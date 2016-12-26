#include "Utils\Logger.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"

#include "Globals.hpp"
#include "Input.hpp"

namespace AnvilEldorado
{
	void QuickBlockInput()
	{
		memset(reinterpret_cast<bool*>(0x238DBEB), 1, Blam::Input::eInputType_Count);
	}

	void QuickUnblockInput()
	{
		memset(reinterpret_cast<bool*>(0x238DBEB), 0, Blam::Input::eInputType_Count);
	}

	void UpdateInputHook()
	{
		auto *s_Input = Input::Instance();

		// If the current context is done, pop it off
		if (s_Input->IsContextDone())
		{
			s_Input->PopContext();
			s_Input->SetContextDone(false);
		}

		auto &s_ContextStack = s_Input->GetContextStack();

		if (!s_ContextStack.empty())
		{
			QuickBlockInput();

			// Tick the active context
			if (!s_ContextStack.top()->GameInputTick())
				s_Input->SetContextDone(true);
			
			QuickUnblockInput();
		}
		else
		{
			auto &s_DefaultHandlers = s_Input->GetDefaultHandlers();

			// Run default handlers
			for (auto &s_DefaultHandler : s_DefaultHandlers)
				s_DefaultHandler();
		}
	}

	void UiInputTick()
	{
		auto *s_Input = Input::Instance();
		auto &s_ContextStack = s_Input->GetContextStack();

		if (s_ContextStack.empty())
			return;

		QuickUnblockInput();
		
		// Tick the active context
		if (!s_ContextStack.top()->UiInputTick())
			s_Input->SetContextDone();

		QuickBlockInput();
	}

	const auto UiPumpWindowMessages = reinterpret_cast<void(*)()>(0x508170);

	void ProcessUiInputHook()
	{
		// Pump Windows messages (replaced function)
		UiPumpWindowMessages();

		UiInputTick();
	}

	const auto UiQuickPumpWindowMessages = reinterpret_cast<void(*)()>(0x42E940);

	void QuickUpdateUiInputHook()
	{
		// Quick pump Windows messages (replaced function)
		UiQuickPumpWindowMessages();

		UiInputTick();
	}

	void BlockInputHook(Blam::Input::InputType p_Type, bool p_Blocked)
	{
		auto *s_Input = Input::Instance();
		auto &s_ContextStack = s_Input->GetContextStack();

		// If a context isn't active, then block input normally,
		// otherwise save the value for when the contexts are done
		if (s_ContextStack.empty())
			BlockInput(p_Type, p_Blocked);

		s_Input->SetInputTypeBlocked(p_Type, p_Blocked);
	}

	void InitBindingsHook(Blam::Input::BindingsTable *p_Bindings)
	{
		*p_Bindings = Input::Instance()->GetBindings();
	}

	// Hook to redirect keybind preference reads to ModuleInput
	Blam::Input::BindingsPreferences* PreferencesGetKeyBindingsHook(Blam::Input::BindingsPreferences *p_Result)
	{
		auto s_Bindings = Input::Instance()->GetBindings();

		memcpy(p_Result->PrimaryKeys, s_Bindings.PrimaryKeys, sizeof(p_Result->PrimaryKeys));
		memcpy(p_Result->PrimaryMouseButtons, s_Bindings.PrimaryMouseButtons, sizeof(p_Result->PrimaryMouseButtons));
		memcpy(p_Result->SecondaryKeys, s_Bindings.SecondaryKeys, sizeof(p_Result->SecondaryKeys));
		memcpy(p_Result->SecondaryMouseButtons, s_Bindings.SecondaryMouseButtons, sizeof(p_Result->SecondaryMouseButtons));

		return p_Result;
	}

	// Hook to redirect keybind preference writes to ModuleInput
	void PreferencesSetKeyBindingsHook(Blam::Input::BindingsPreferences p_NewBindings)
	{
		auto *s_Input = Input::Instance();
		auto s_Bindings = s_Input->GetBindings();

		memcpy(s_Bindings.PrimaryKeys, p_NewBindings.PrimaryKeys, sizeof(s_Bindings.PrimaryKeys));
		memcpy(s_Bindings.PrimaryMouseButtons, p_NewBindings.PrimaryMouseButtons, sizeof(s_Bindings.PrimaryMouseButtons));
		memcpy(s_Bindings.SecondaryKeys, p_NewBindings.SecondaryKeys, sizeof(s_Bindings.SecondaryKeys));
		memcpy(s_Bindings.SecondaryMouseButtons, p_NewBindings.SecondaryMouseButtons, sizeof(s_Bindings.SecondaryMouseButtons));

		s_Input->UpdateBindings();
		
		// TODO: Modules::CommandMap::Instance().ExecuteCommand("WriteConfig");
	}

	// Hook to prevent the game from resetting keybindings when we don't want it to
	void GetDefaultBindingsHook(int32_t p_Type, Blam::Input::BindingsTable *p_Result)
	{
		*p_Result = Input::Instance()->GetBindings();
	}

	Blam::Input::InputType g_ActionInputTypes[Blam::Input::eGameAction_KeyboardMouseCount] =
	{
		Blam::Input::eInputTypeUi,      // eGameActionUiLeftTrigger
		Blam::Input::eInputTypeUi,      // eGameActionUiRightTrigger
		Blam::Input::eInputTypeUi,      // eGameActionUiUp
		Blam::Input::eInputTypeUi,      // eGameActionUiDown
		Blam::Input::eInputTypeUi,      // eGameActionUiLeft
		Blam::Input::eInputTypeUi,      // eGameActionUiRight
		Blam::Input::eInputTypeSpecial, // eGameActionUiStart
		Blam::Input::eInputTypeSpecial, // eGameActionUiSelect
		Blam::Input::eInputTypeUi,      // eGameActionUiLeftStick
		Blam::Input::eInputTypeUi,      // eGameActionUiRightStick
		Blam::Input::eInputTypeUi,      // eGameActionUiA
		Blam::Input::eInputTypeUi,      // eGameActionUiB
		Blam::Input::eInputTypeUi,      // eGameActionUiX
		Blam::Input::eInputTypeUi,      // eGameActionUiY
		Blam::Input::eInputTypeUi,      // eGameActionUiLeftBumper
		Blam::Input::eInputTypeUi,      // eGameActionUiRightBumper
		Blam::Input::eInputTypeGame,    // eGameActionJump
		Blam::Input::eInputTypeGame,    // eGameActionSwitchGrenades
		Blam::Input::eInputTypeGame,    // eGameActionSwitchWeapons
		Blam::Input::eInputTypeGame,    // eGameActionUnk19
		Blam::Input::eInputTypeGame,    // eGameActionReloadRight
		Blam::Input::eInputTypeGame,    // eGameActionUse
		Blam::Input::eInputTypeGame,    // eGameActionReloadLeft
		Blam::Input::eInputTypeGame,    // eGameActionPickUpLeft
		Blam::Input::eInputTypeGame,    // eGameActionMelee
		Blam::Input::eInputTypeGame,    // eGameActionThrowGrenade
		Blam::Input::eInputTypeGame,    // eGameActionFireRight
		Blam::Input::eInputTypeGame,    // eGameActionFireLeft
		Blam::Input::eInputTypeGame,    // eGameActionMeleeFire
		Blam::Input::eInputTypeGame,    // eGameActionCrouch
		Blam::Input::eInputTypeGame,    // eGameActionZoom
		Blam::Input::eInputTypeGame,    // eGameActionUnk31
		Blam::Input::eInputTypeGame,    // eGameActionUnk32
		Blam::Input::eInputTypeGame,    // eGameActionSprint
		Blam::Input::eInputTypeGame,    // eGameActionUnk34
		Blam::Input::eInputTypeGame,    // eGameActionUnk35
		Blam::Input::eInputTypeGame,    // eGameActionUnk36
		Blam::Input::eInputTypeGame,    // eGameActionUnk37
		Blam::Input::eInputTypeGame,    // eGameActionUnk38
		Blam::Input::eInputTypeUi,      // eGameActionGeneralChat
		Blam::Input::eInputTypeUi,      // eGameActionTeamChat
		Blam::Input::eInputTypeGame,    // eGameActionUnk41
		Blam::Input::eInputTypeGame,    // eGameActionUnk42
		Blam::Input::eInputTypeGame,    // eGameActionUnk43
		Blam::Input::eInputTypeGame,    // eGameActionUseConsumable1
		Blam::Input::eInputTypeGame,    // eGameActionUseConsumable2
		Blam::Input::eInputTypeGame,    // eGameActionUseConsumable3
		Blam::Input::eInputTypeGame,    // eGameActionUseConsumable4
		Blam::Input::eInputTypeGame,    // eGameActionVehicleBoost
		Blam::Input::eInputTypeGame,    // eGameActionVehicleDive
		Blam::Input::eInputTypeGame,    // eGameActionVehicleRaise
		Blam::Input::eInputTypeGame,    // eGameActionVehicleAccelerate
		Blam::Input::eInputTypeGame,    // eGameActionVehicleBrake
		Blam::Input::eInputTypeGame,    // eGameActionVehicleFire
		Blam::Input::eInputTypeGame,    // eGameActionVehicleAltFire
		Blam::Input::eInputTypeGame,    // eGameActionVehicleExit
		Blam::Input::eInputTypeUi,      // eGameActionUnk56
		Blam::Input::eInputTypeUi,      // eGameActionUnk57
		Blam::Input::eInputTypeUi,      // eGameActionUnk58
		Blam::Input::eInputTypeGame,    // eGameActionMoveForward
		Blam::Input::eInputTypeGame,    // eGameActionMoveBack
		Blam::Input::eInputTypeGame,    // eGameActionMoveLeft
		Blam::Input::eInputTypeGame,    // eGameActionMoveRight
	};

	// Hook to get keyboard action types from the actionInputTypes array
	// instead of using hardcoded values
	__declspec(naked) void GetKeyboardActionTypeHook()
	{
		__asm
		{
			// ecx has the action index
			// eax needs to contain the type on return
			// ecx needs to be 0 on return
			mov eax, g_ActionInputTypes[ecx * 4]
			xor ecx, ecx
			push 0x60C51E
			ret
		}
	}

	const auto GetControllerState = reinterpret_cast<int8_t(*)(int32_t, int32_t, void *)>(0x65EF60);

	char GetControllerStateHook(int32_t p_UserIndex, int32_t p_Arg2, void *p_Arg3)
	{
		return GetControllerState(Input::Instance()->GetControllerPort(), p_Arg2, p_Arg3);
	}

	const auto SetControllerVibration = reinterpret_cast<int8_t(*)(int32_t, int32_t, int8_t)>(0x65F220);

	uint32_t SetControllerVibrationHook(int32_t p_UserIndex, int32_t p_Arg2, int8_t p_Arg3)
	{
		return SetControllerVibration(Input::Instance()->GetControllerPort(), p_Arg2, p_Arg3);
	}

	const auto EngineProcessKeyBindings = reinterpret_cast<void(*)(const Blam::Input::BindingsTable &, Blam::Input::ActionState *)>(0x60C4A0);

	void ProcessKeyBindingsHook(const Blam::Input::BindingsTable &p_Bindings, Blam::Input::ActionState *p_Actions)
	{
		EngineProcessKeyBindings(p_Bindings, p_Actions);

		// Unset the "handled" flag for inactive actions
		for (auto i = 0; i < Blam::Input::eGameAction_KeyboardMouseCount; i++)
		{
			if (p_Actions[i].Ticks == 0)
				p_Actions[i].Flags &= ~Blam::Input::eActionStateFlagsHandled;
		}
	}

	bool Input::Init()
	{
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Patch;

		LoadDefaultBindings();

		return Hook(0x1129A0, UpdateInputHook).Apply()
			&& Hook(0x105CBA, ProcessUiInputHook, HookFlags::IsCall).Apply()
			&& Hook(0x106417, QuickUpdateUiInputHook, HookFlags::IsCall).Apply()
			&& Hook(0x234238, BlockInputHook, HookFlags::IsCall).Apply()
			&& Hook(0x20BF00, InitBindingsHook).Apply()
			&& Hook(0x10AB40, PreferencesGetKeyBindingsHook).Apply()
			&& Hook(0x10D040, PreferencesSetKeyBindingsHook).Apply()
			&& Hook(0x20C040, GetDefaultBindingsHook).Apply()
			&& Hook(0x20C4F6, GetKeyboardActionTypeHook).Apply()
			&& Hook(0x1128FB, GetControllerStateHook, HookFlags::IsCall).Apply()
			&& Hook(0x11298B, SetControllerVibrationHook, HookFlags::IsCall).Apply()
			// Prevent the game from forcing certain binds on load
			&& Patch::NopFill(0x6A225B, 2)
			// Disable saber's ui input code
			&& Patch(0x6940E7, { 0x90, 0xE9 }).Apply()
			// Never clip the cursor to the window boundaries
			&& Patch::NopFill(0x112613, 2)
			// Fix a bug in the keyboard input routine that screws up UI keys.
			// If an action has the "handled" flag set and has a secondary key
			// bound to it, then the tick count gets reset to 0 because the
			// secondary key won't be down and the action gets spammed.
			//
			// To fix it, we have to nop out every place where the flag is
			// checked, disable the code which updates the flag, and then
			// update it ourselves only after every key has been checked.
			&& Patch::NopFill(0x20C529, 2)
			&& Patch::NopFill(0x20C558, 2)
			&& Patch::NopFill(0x20C591, 2)
			&& Patch::NopFill(0x20C5C2, 2)
			&& Patch::NopFill(0x20C5FA, 2)
			&& Patch::NopFill(0x20C63F, 2)
			&& Patch(0x20C69F, { 0xEB }).Apply()
			&& Hook(0x20D980, ProcessKeyBindingsHook, HookFlags::IsCall).Apply();
	}

	std::stack<std::shared_ptr<InputContext>> &Input::GetContextStack()
	{
		return m_ContextStack;
	}

	void Input::PushContext(std::shared_ptr<InputContext> p_Context)
	{
		if (m_ContextStack.empty())
		{
			// Block all input, unacquiring the mouse in the process
			for (auto i = 0; i < Blam::Input::eInputType_Count; i++)
				BlockInput(static_cast<Blam::Input::InputType>(i), true);
		}
		else
		{
			// Deactivate the current context
			m_ContextStack.top()->Deactivated();
		}

		// Push and activate the new context
		m_ContextStack.push(p_Context);
		p_Context->Activated();
	}

	void Input::PopContext()
	{
		m_ContextStack.top()->Deactivated();
		m_ContextStack.pop();

		if (!m_ContextStack.empty())
		{
			// Activate the previous context
			m_ContextStack.top()->Activated();
		}
		else
		{
			// Restore the game's input block states
			for (auto i = 0; i < Blam::Input::eInputType_Count; i++)
				BlockInput(static_cast<Blam::Input::InputType>(i), m_BlockStates[i]);
		}
	}

	bool Input::IsContextDone() const
	{
		return m_ContextDone;
	}

	void Input::SetContextDone(const bool p_Done)
	{
		m_ContextDone = p_Done;
	}
	
	void Input::SetInputTypeBlocked(const Blam::Input::InputType &p_Type, const bool p_Blocked)
	{
		m_BlockStates[p_Type] = p_Blocked;
	}

	std::vector<Input::DefaultInputHandler> &Input::GetDefaultHandlers()
	{
		return m_DefaultHandlers;
	}

	void Input::RegisterDefaultInputHandler(Input::DefaultInputHandler p_Handler)
	{
		m_DefaultHandlers.push_back(p_Handler);
	}

	void Input::SetKeyboardSettingsMenu(
		const std::vector<Blam::Input::ConfigurableAction> &p_InfantrySettings,
		const std::vector<Blam::Input::ConfigurableAction> &p_VehicleSettings)
	{
		// The settings array needs to have infantry settings followed by
		// vehicle settings due to assumptions that the EXE makes
		m_Settings.clear();
		m_Settings.insert(m_Settings.end(), p_InfantrySettings.begin(), p_InfantrySettings.end());
		m_Settings.insert(m_Settings.end(), p_VehicleSettings.begin(), p_VehicleSettings.end());

		// Patch the exe to point to the new menus
		auto s_InfantryCount = p_InfantrySettings.size();
		auto s_VehicleCount = p_VehicleSettings.size();

		uint32_t s_InfantryPointers[] = { 0x398573, 0x3993CF, 0x39A42F, 0x39B4B0 };
		uint32_t s_VehiclePointers[] = { 0x39856D, 0x3993CA, 0x39A63A, 0x39A645, 0x39B4AB };
		uint32_t s_EndPointers[] = { 0x39A84A };
		uint32_t s_InfantryCountPointers[] = { 0x39857B, 0x3993BC, 0x39B495 };
		uint32_t s_VehicleCountPointers[] = { 0x398580, 0x3993C1, 0x39B49A };

		for (auto s_Offset : s_InfantryPointers)
			*(Blam::Input::ConfigurableAction **)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset) = &m_Settings[0];

		for (auto s_Offset : s_VehiclePointers)
			*(Blam::Input::ConfigurableAction **)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset) = &m_Settings[0] + s_InfantryCount;

		for (auto s_Offset : s_EndPointers)
			*(Blam::Input::ConfigurableAction **)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset) = &m_Settings[0] + m_Settings.size();

		for (auto s_Offset : s_InfantryCountPointers)
			*(int32_t *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset) = static_cast<int>(s_InfantryCount);

		for (auto s_Offset : s_VehicleCountPointers)
			*(int32_t *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + s_Offset) = static_cast<int>(s_VehicleCount);
	}

	bool Input::RawInputEnabled() const
	{
		return m_RawInputEnabled;
	}

	void Input::SetRawInputEnabled(const bool p_Enabled)
	{
		// TODO: Update raw input state here...

		m_RawInputEnabled = p_Enabled;
	}

	int32_t Input::GetControllerPort() const
	{
		return m_ControllerPort;
	}

	void Input::SetControllerPort(const int32_t p_Port)
	{
		// TODO: Update internal controller port here...

		m_ControllerPort = p_Port;
	}

	float Input::GetControllerHorizontalSensitivity() const
	{
		// TODO: Get controller horizontal sensitivity...

		return 0.0f;
	}

	void Input::SetControllerHorizontalSensitivity(const float p_Sensitivity)
	{
		// TODO: Update controller horizontal sensitivity here...
	}

	float Input::GetControllerVerticalSensitivity() const
	{
		// TODO: Get controller vertical sensitivity...

		return 0.0f;
	}

	void Input::SetControllerVerticalSensitivity(const float p_Sensitivity)
	{
		// TODO: Update controller vertical sensitivity here...
	}

	Blam::Input::BindingsTable &Input::GetBindings()
	{
		return m_Bindings;
	}

	void Input::LoadDefaultBindings()
	{
		memset(&m_Bindings, 0, sizeof(m_Bindings));

		m_Bindings.ControllerSensitivityX = 120.f;
		m_Bindings.ControllerSensitivityY = 60.f;
		m_Bindings.Unknown200 = .8f;
		m_Bindings.Unknown204 = 1.f;

		// Set all bindings to "none"
		for (auto i = 0; i < Blam::Input::eGameAction_KeyboardMouseCount; i++)
		{
			if (i < Blam::Input::eGameAction_ControllerCount)
				m_Bindings.ControllerButtons[i] = Blam::Input::eControllerButton_None;
			m_Bindings.PrimaryKeys[i] = Blam::Input::eKeyCode_None;
			m_Bindings.SecondaryKeys[i] = Blam::Input::eKeyCode_None;
			m_Bindings.PrimaryMouseButtons[i] = Blam::Input::eMouseButton_None;
			m_Bindings.SecondaryMouseButtons[i] = Blam::Input::eMouseButton_None;
		}

		// Bind UI actions directly to their controller buttons
		// The ControllerButton enum lines up with the beginning of the
		// GameAction enum, and the game itself even uses a for loop like this
		for (auto i = 0; i < Blam::Input::eControllerButton_Count; i++)
			m_Bindings.ControllerButtons[i] = static_cast<Blam::Input::ControllerButton>(i);

		// Default controller bindings

		m_Bindings.ControllerButtons[Blam::Input::eGameActionJump] = Blam::Input::eControllerButtonA;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionSwitchGrenades] = Blam::Input::eControllerButtonDpadRight;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionSwitchWeapons] = Blam::Input::eControllerButtonY;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk19] = Blam::Input::eControllerButtonY;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionReloadRight] = Blam::Input::eControllerButtonRightBumper;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUse] = Blam::Input::eControllerButtonRightBumper;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionReloadLeft] = Blam::Input::eControllerButtonLeftBumper;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionPickUpLeft] = Blam::Input::eControllerButtonLeftBumper;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionMelee] = Blam::Input::eControllerButtonB;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionThrowGrenade] = Blam::Input::eControllerButtonLeftTrigger;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionFireRight] = Blam::Input::eControllerButtonRightTrigger;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionFireLeft] = Blam::Input::eControllerButtonLeftTrigger;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionCrouch] = Blam::Input::eControllerButtonX;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionZoom] = Blam::Input::eControllerButtonRightStick;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk31] = Blam::Input::eControllerButtonDpadLeft;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk32] = Blam::Input::eControllerButtonDpadRight;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionSprint] = Blam::Input::eControllerButtonLeftStick;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk34] = Blam::Input::eControllerButtonX;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk35] = Blam::Input::eControllerButtonDpadDown;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk36] = Blam::Input::eControllerButtonDpadUp;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk37] = Blam::Input::eControllerButtonDpadLeft;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk38] = Blam::Input::eControllerButtonDpadRight;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk41] = Blam::Input::eControllerButtonDpadUp;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk42] = Blam::Input::eControllerButtonA;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionVehicleBoost] = Blam::Input::eControllerButtonLeftTrigger;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionVehicleDive] = Blam::Input::eControllerButtonX;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionVehicleRaise] = Blam::Input::eControllerButtonA;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionVehicleFire] = Blam::Input::eControllerButtonRightTrigger;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionVehicleAltFire] = Blam::Input::eControllerButtonLeftTrigger;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionMeleeFire] = Blam::Input::eControllerButtonB;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk56] = Blam::Input::eControllerButtonDpadDown;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk57] = Blam::Input::eControllerButtonDpadUp;
		m_Bindings.ControllerButtons[Blam::Input::eGameActionUnk58] = Blam::Input::eControllerButtonA;

		m_Bindings.ControllerHoldButtons[Blam::Input::eGameActionUnk19] = true;
		m_Bindings.ControllerHoldButtons[Blam::Input::eGameActionUse] = true;
		m_Bindings.ControllerHoldButtons[Blam::Input::eGameActionPickUpLeft] = true;

		// Default keyboard/mouse bindings

		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiUp] = Blam::Input::eKeyCodeUp;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiDown] = Blam::Input::eKeyCodeDown;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiLeft] = Blam::Input::eKeyCodeLeft;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiRight] = Blam::Input::eKeyCodeRight;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiStart] = Blam::Input::eKeyCodeEscape;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiSelect] = Blam::Input::eKeyCodeTab;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiA] = Blam::Input::eKeyCodeEnter;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionUiA] = Blam::Input::eKeyCodeSpace;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiB] = Blam::Input::eKeyCodeEscape;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionUiB] = Blam::Input::eKeyCodeBack;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiX] = Blam::Input::eKeyCodeX;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiY] = Blam::Input::eKeyCodeDelete;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiLeftBumper] = Blam::Input::eKeyCodeQ;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUiRightBumper] = Blam::Input::eKeyCodeE;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionJump] = Blam::Input::eKeyCodeSpace;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionSwitchGrenades] = Blam::Input::eKeyCodeG;
		m_Bindings.PrimaryMouseButtons[Blam::Input::eGameActionSwitchWeapons] = Blam::Input::eMouseButtonWheelUp;
		m_Bindings.SecondaryMouseButtons[Blam::Input::eGameActionSwitchWeapons] = Blam::Input::eMouseButtonWheelDown;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionReloadRight] = Blam::Input::eKeyCodeR;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUse] = Blam::Input::eKeyCodeE;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionReloadLeft] = Blam::Input::eKeyCodeZ; // TODO: Find a better key?
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionPickUpLeft] = Blam::Input::eKeyCodeZ; // TODO: Find a better key?
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionMelee] = Blam::Input::eKeyCodeQ;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionMelee] = Blam::Input::eKeyCodeV;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionThrowGrenade] = Blam::Input::eKeyCodeF;
		m_Bindings.SecondaryMouseButtons[Blam::Input::eGameActionThrowGrenade] = Blam::Input::eMouseButtonMiddle;
		m_Bindings.PrimaryMouseButtons[Blam::Input::eGameActionFireRight] = Blam::Input::eMouseButtonLeft;
		m_Bindings.PrimaryMouseButtons[Blam::Input::eGameActionFireLeft] = Blam::Input::eMouseButtonRight;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionCrouch] = Blam::Input::eKeyCodeLControl;
		m_Bindings.PrimaryMouseButtons[Blam::Input::eGameActionZoom] = Blam::Input::eMouseButtonRight;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionSprint] = Blam::Input::eKeyCodeLShift;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionGeneralChat] = Blam::Input::eKeyCodeT;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionTeamChat] = Blam::Input::eKeyCodeY;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUnk41] = Blam::Input::eKeyCodeCapital;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUnk42] = Blam::Input::eKeyCodeSpace;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionVehicleBoost] = Blam::Input::eKeyCodeLShift;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionVehicleDive] = Blam::Input::eKeyCodeLControl;
		m_Bindings.SecondaryMouseButtons[Blam::Input::eGameActionVehicleDive] = Blam::Input::eMouseButtonWheelDown;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionVehicleRaise] = Blam::Input::eKeyCodeSpace;
		m_Bindings.SecondaryMouseButtons[Blam::Input::eGameActionVehicleRaise] = Blam::Input::eMouseButtonWheelUp;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionVehicleAccelerate] = Blam::Input::eKeyCodeW;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionVehicleAccelerate] = Blam::Input::eKeyCodeUp;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionVehicleBrake] = Blam::Input::eKeyCodeS;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionVehicleBrake] = Blam::Input::eKeyCodeDown;
		m_Bindings.PrimaryMouseButtons[Blam::Input::eGameActionVehicleFire] = Blam::Input::eMouseButtonLeft;
		m_Bindings.PrimaryMouseButtons[Blam::Input::eGameActionVehicleAltFire] = Blam::Input::eMouseButtonRight;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionMeleeFire] = Blam::Input::eKeyCodeQ;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionMeleeFire] = Blam::Input::eKeyCodeV;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUnk56] = Blam::Input::eKeyCodeD;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUnk57] = Blam::Input::eKeyCodeA;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionUnk58] = Blam::Input::eKeyCodeSpace;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionMoveForward] = Blam::Input::eKeyCodeW;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionMoveForward] = Blam::Input::eKeyCodeUp;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionMoveBack] = Blam::Input::eKeyCodeS;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionMoveBack] = Blam::Input::eKeyCodeDown;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionMoveLeft] = Blam::Input::eKeyCodeA;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionMoveLeft] = Blam::Input::eKeyCodeLeft;
		m_Bindings.PrimaryKeys[Blam::Input::eGameActionMoveRight] = Blam::Input::eKeyCodeD;
		m_Bindings.SecondaryKeys[Blam::Input::eGameActionMoveRight] = Blam::Input::eKeyCodeRight;

		UpdateBindings();
	}

	void Input::UpdateBindings()
	{
		CopyBinding(Blam::Input::eGameActionUse, Blam::Input::eGameActionVehicleExit);
		Blam::Input::SetBindings(0, m_Bindings);
	}

	void Input::CopyBinding(const Blam::Input::GameAction &p_SourceAction, const Blam::Input::GameAction &p_DestAction)
	{
		m_Bindings.ControllerButtons[p_DestAction] = m_Bindings.ControllerButtons[p_SourceAction];
		m_Bindings.ControllerHoldButtons[p_DestAction] = m_Bindings.ControllerHoldButtons[p_SourceAction];
		m_Bindings.PrimaryKeys[p_DestAction] = m_Bindings.PrimaryKeys[p_SourceAction];
		m_Bindings.SecondaryKeys[p_DestAction] = m_Bindings.SecondaryKeys[p_SourceAction];
		m_Bindings.PrimaryMouseButtons[p_DestAction] = m_Bindings.PrimaryMouseButtons[p_SourceAction];
		m_Bindings.SecondaryMouseButtons[p_DestAction] = m_Bindings.SecondaryMouseButtons[p_SourceAction];
	}

	bool Input::BindControllerAction(const Blam::Input::GameAction &p_GameAction, const Blam::Input::ControllerButton &p_ControllerButton)
	{
		m_Bindings.ControllerButtons[p_GameAction] = p_ControllerButton;
		UpdateBindings();

		return true;
	}

	bool Input::BindKeyboardAction(const Blam::Input::GameAction &p_GameAction, const Blam::Input::KeyCode &p_PrimaryKey, const Blam::Input::KeyCode &p_SecondaryKey)
	{
		auto s_PrimaryKey = p_PrimaryKey;
		auto s_SecondaryKey = p_SecondaryKey;

		if ((s_PrimaryKey == Blam::Input::eKeyCode_None) && (s_SecondaryKey != Blam::Input::eKeyCode_None))
		{
			s_PrimaryKey = s_SecondaryKey;
			s_SecondaryKey = Blam::Input::eKeyCode_None;
		}

		m_Bindings.PrimaryKeys[p_GameAction] = s_PrimaryKey;
		m_Bindings.SecondaryKeys[p_GameAction] = s_SecondaryKey;

		UpdateBindings();

		return true;
	}

	bool Input::BindMouseAction(const Blam::Input::GameAction &p_GameAction, const Blam::Input::MouseButton &p_PrimaryButton, const Blam::Input::MouseButton &p_SecondaryButton)
	{
		if (p_GameAction >= Blam::Input::eGameAction_ControllerCount)
		{
			WriteLog("ERROR: Movement actions cannot be bound to mouse buttons.");
			return false;
		}

		auto s_PrimaryButton = p_PrimaryButton;
		auto s_SecondaryButton = p_SecondaryButton;

		if ((s_PrimaryButton == Blam::Input::eMouseButton_None) && (s_SecondaryButton != Blam::Input::eMouseButton_None))
		{
			s_PrimaryButton = s_SecondaryButton;
			s_SecondaryButton = Blam::Input::eMouseButton_None;
		}

		m_Bindings.PrimaryMouseButtons[p_GameAction] = s_PrimaryButton;
		m_Bindings.SecondaryMouseButtons[p_GameAction] = s_SecondaryButton;

		return true;
	}
}