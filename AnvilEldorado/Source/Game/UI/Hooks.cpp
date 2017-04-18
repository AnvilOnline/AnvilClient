#include "Hooks.hpp"
#include <Interfaces\Client.hpp>
#include <Interfaces\Engine.hpp>

#include <BuildInfo.hpp>

#include <string>

using namespace AnvilEldorado::Game::UI;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	auto s_Address = s_Base + 0x0051E040;
	HookFunctionOffset(s_Address, LocalizeString);

	return true;
}

DeclareDetouredFunction(Hooks, bool, __cdecl, LocalizeString, int p_TagIndex, Blam::Text::StringID p_StringId, wchar_t* p_Output)
{
	const static size_t MaxStringLength = 0x400;

	if (p_StringId == "start_new_campaign")
	{
#if _DEBUG
		swprintf_s(p_Output, MaxStringLength, L"Anvil: Online - Dev Build - %d", ANVIL_BUILD);
#else
		swprintf_s(p_Output, MaxStringLength, L"Anvil: Online - %d", ANVIL_BUILD);
#endif
		return true;
	}

	return o_LocalizeString(p_TagIndex, p_StringId, p_Output);
}