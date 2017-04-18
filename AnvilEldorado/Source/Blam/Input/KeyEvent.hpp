#pragma once
#include "KeyCode.hpp"
#include "KeyEventModifiers.hpp"
#include "KeyEventType.hpp"

namespace Blam::Input
{
	struct KeyEvent
	{
		KeyEventModifiers Modifiers; // Bitfield of modifier keys that are down
		KeyEventType Type;           // Event type
		KeyCode Code;                // The key code, or -1 if unavailable
		char16_t Char;               // For eKeyEventTypeChar events, the character that was typed, or -1 if unavailable
		bool PreviousState;          // If true, the key was down before this event happened
	};
	static_assert(sizeof(KeyEvent) == 0x10, "Invalid KeyEvent size");
}