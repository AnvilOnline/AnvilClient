#include <Client/Hooks/EngineHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(EngineHooks, int, GameTick, __stdcall)
{
	auto s_Ret = o_GameTick();

	__asm NOP;

	return s_Ret;
}