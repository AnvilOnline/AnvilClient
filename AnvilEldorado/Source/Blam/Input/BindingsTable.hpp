#pragma once
#include "ControllerButton.hpp"
#include "GameAction.hpp"
#include "KeyCode.hpp"
#include "MouseButton.hpp"

namespace Blam::Input
{
	struct BindingsTable
	{
		float ControllerSensitivityX; // Default = 120
		float ControllerSensitivityY; // Default = 60
		ControllerButton ControllerButtons[eGameAction_ControllerCount];
		bool ControllerHoldButtons[eGameAction_ControllerCount]; // true if a button needs to be held down? just a guess
		KeyCode PrimaryKeys[eGameAction_KeyboardMouseCount];
		KeyCode SecondaryKeys[eGameAction_KeyboardMouseCount];
		MouseButton PrimaryMouseButtons[eGameAction_KeyboardMouseCount];
		MouseButton SecondaryMouseButtons[eGameAction_KeyboardMouseCount];
		uint32_t Unknown1F8;
		uint32_t Unknown1FC;
		float Unknown200; // Default = .8
		float Unknown204; // Default = 1
	};
	static_assert(sizeof(BindingsTable) == 0x208, "Invalid BindingsTable size");
}