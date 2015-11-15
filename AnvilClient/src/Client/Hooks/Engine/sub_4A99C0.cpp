// TODO: Figure out what this does, or remove it.
#include <Client/Hooks/EngineHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(EngineHooks, int, sub_4A99C0, __fastcall, void* p_Structure)
{
	WriteLog("MapName: %s.", static_cast<char*>(p_Structure) + 0x24);
	WriteLog("GameType: %d.", static_cast<char*>(p_Structure) + 0x32C);

	auto s_Ret = o_sub_4A99C0(p_Structure);

	//__asm INT 3;

	return s_Ret;
}