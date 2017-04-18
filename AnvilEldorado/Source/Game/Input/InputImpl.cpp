#include "InputImpl.hpp"

#include <Blam\Input\ActionState.hpp>
#include <Blam\Input\GameAction.hpp>

using namespace AnvilEldorado::Game::Input;
using namespace Blam::Input;

bool InputImpl::Init()
{
	// TODO: Init
	return true;
}

uint8_t InputImpl::GetKeyTicks(KeyCode key, InputType type)
{
	typedef uint8_t(*EngineGetKeyTicksPtr)(KeyCode, InputType);
	auto EngineGetKeyTicks = reinterpret_cast<EngineGetKeyTicksPtr>(0x511B60);
	return EngineGetKeyTicks(key, type);
}

uint16_t InputImpl::GetKeyMs(KeyCode key, InputType type)
{
	typedef uint8_t(*EngineGetKeyMsPtr)(KeyCode, InputType);
	auto EngineGetKeyMs = reinterpret_cast<EngineGetKeyMsPtr>(0x511CE0);
	return EngineGetKeyMs(key, type);
}

bool InputImpl::ReadKeyEvent(KeyEvent* result, InputType type)
{
	typedef bool(*EngineReadKeyEventPtr)(KeyEvent*, InputType);
	auto EngineReadKeyEvent = reinterpret_cast<EngineReadKeyEventPtr>(0x5118C0);
	return EngineReadKeyEvent(result, type);
}

void InputImpl::BlockInput(InputType type, bool block)
{
	typedef uint8_t(*EngineBlockInputPtr)(InputType, bool);
	auto EngineBlockInput = reinterpret_cast<EngineBlockInputPtr>(0x512530);
	EngineBlockInput(type, block);
}

bool InputImpl::GetBindings(int localPlayerIndex, BindingsTable *result)
{
	if (localPlayerIndex < 0 || localPlayerIndex >= 4)
		return false;
	typedef void(*EngineGetBindingsPtr)(int localPlayerIndex, BindingsTable *result);
	auto EngineGetBindings = reinterpret_cast<EngineGetBindingsPtr>(0x60BE70);
	EngineGetBindings(localPlayerIndex, result);
	return true;
}

bool InputImpl::SetBindings(int localPlayerIndex, const BindingsTable &newBindings)
{
	if (localPlayerIndex < 0 || localPlayerIndex >= 4)
		return false;
	typedef void(*EngineSetBindingsPtr)(int localPlayerIndex, const BindingsTable &newBindings);
	auto EngineSetBindings = reinterpret_cast<EngineSetBindingsPtr>(0x60D830);
	EngineSetBindings(localPlayerIndex, newBindings);
	return true;
}

Blam::Input::ActionState* InputImpl::GetActionState(Blam::Input::GameAction action)
{
	if (action < 0 || action >= eGameAction_KeyboardMouseCount)
		return nullptr;
	auto actions = reinterpret_cast<ActionState*>(0x244D1F0);
	return &actions[action];
}