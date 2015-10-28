// TODO: Figure out what this does, or remove it.
#include <Client/Hooks/EngineHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(EngineHooks, void*, sub_5C7E40, __stdcall)
{
	auto s_Ret = o_sub_5C7E40();

	//__asm INT 3;

	return s_Ret;
}