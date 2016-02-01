#include <Client/Hooks/EngineHooks.hpp>

using namespace Anvil::Client::Hooks;

// TODO: Remove this?
HookedFunction(EngineHooks, int, GameTick, __stdcall)
{
	auto s_Ret = o_GameTick();

	// This does not get called continuously, only when certain things are ticking (maybe ingame?)
	__asm NOP;

	return s_Ret;
}