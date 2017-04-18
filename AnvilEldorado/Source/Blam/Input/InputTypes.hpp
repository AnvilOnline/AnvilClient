#pragma once
#include "KeyCode.hpp"
#include "KeyEvent.hpp"
#include "InputType.hpp"
#include "BindingsTable.hpp"
#include "ActionState.hpp"

namespace Blam::Input
{
	// Gets the number of ticks that a key has been held down for.
	// Will always be nonzero if the key is down.
	uint8_t GetKeyTicks(KeyCode key, InputType type);

	// Gets the number of milliseconds that a key has been held down for.
	// Will always be nonzero if the key is down.
	uint16_t GetKeyMs(KeyCode key, InputType type);

	// Reads a raw keyboard input event. Returns false if nothing is
	// available. You should call this in a loop to ensure that you process
	// all available events. NOTE THAT THIS IS ONLY GUARANTEED TO WORK
	// AFTER WINDOWS MESSAGES HAVE BEEN PUMPED IN THE UPDATE CYCLE. ALSO,
	// THIS WILL NOT WORK IF UI INPUT IS DISABLED, REGARDLESS OF THE INPUT
	// TYPE YOU SPECIFY.
	bool ReadKeyEvent(KeyEvent* result, InputType type);

	// Blocks or unblocks an input type.
	void BlockInput(InputType type, bool block);

	// Gets the input bindings table for a local player.
	// Halo Online only uses index 0, but there are 4 total.
	// This will fail if the index is out-of-range.
	bool GetBindings(int localPlayerIndex, BindingsTable *result);

	// Updates the input bindings table for a local player.
	// Halo Online only uses index 0, but there are 4 total.
	// This will fail if the index is out-of range.
	bool SetBindings(int localPlayerIndex, const BindingsTable &newBindings);

	// Gets the state of the button bound to an action.
	// Will return null if the action is invalid.
	ActionState* GetActionState(GameAction action);
}