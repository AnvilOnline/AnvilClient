#pragma once
#include <cstdint>
#include "Controller.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Actions.hpp"

namespace Blam::Input
{
	struct BindingsTable
	{
		float ControllerSensitivityX;
		float ControllerSensitivityY;
		ControllerButton ControllerButtons[(int)Action::ControllerCount];
		bool ControllerHoldButtons[(int)Action::ControllerCount];
		KeyCode PrimaryKeys[(int)Action::KeyboardMouseCount];
		KeyCode SecondaryKeys[(int)Action::KeyboardMouseCount];
		MouseButton PrimaryMouseButtons[(int)Action::KeyboardMouseCount];
		MouseButton SecondaryMouseButtons[(int)Action::KeyboardMouseCount];
		uint32_t Unknown1F8;
		uint32_t Unknown1FC;
		float Unknown200;
		float Unknown204;
	};
	static_assert(sizeof(BindingsTable) == 0x208, "Blam::Input::BindingsTable");

	struct BindingsPreferences
	{
		KeyCode PrimaryKeys[(int)Action::KeyboardMouseCount];
		MouseButton PrimaryMouseButtons[(int)Action::KeyboardMouseCount];
		KeyCode SecondaryKeys[(int)Action::KeyboardMouseCount];
		MouseButton SecondaryMouseButtons[(int)Action::KeyboardMouseCount];
	};
	static_assert(sizeof(BindingsPreferences) == 0x17C, "Blam::Input::BindingsPreferences");

	bool GetBindings(int p_LocalPlayerIndex, BindingsTable *p_Result);

	bool SetBindings(int p_LocalPlayerIndex, const BindingsTable &p_NewBindings);
}