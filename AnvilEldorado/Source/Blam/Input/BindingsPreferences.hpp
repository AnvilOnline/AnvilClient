#pragma once
#include "GameAction.hpp"
#include "KeyCode.hpp"
#include "MouseButton.hpp"

namespace AnvilEldorado::Blam::Input
{
	struct BindingsPreferences
	{
		KeyCode PrimaryKeys[eGameAction_KeyboardMouseCount];
		MouseButton PrimaryMouseButtons[eGameAction_KeyboardMouseCount];
		KeyCode SecondaryKeys[eGameAction_KeyboardMouseCount];
		MouseButton SecondaryMouseButtons[eGameAction_KeyboardMouseCount];
	};

#if _DEBUG
	static_assert(sizeof(BindingsPreferences) == 0x17C, "Invalid BindingsPreferences size");
#endif
}