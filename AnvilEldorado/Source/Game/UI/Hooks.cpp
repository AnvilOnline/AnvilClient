#include "Hooks.hpp"
#include <Interfaces\Client.hpp>
#include <Interfaces\Engine.hpp>

#include <BuildInfo.hpp>

#include <Game\UI\UIImpl.hpp>

using namespace AnvilEldorado::Game::UI;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	auto s_Address = s_Base + 0x0011E040;
	HookFunctionOffset(s_Address, LocalizeString);

	s_Address = s_Base + 0x006DF6E0;
	HookFunctionOffset(s_Address, MainMenuUpdate);

	return true;
}

DeclareDetouredFunction(Hooks, bool, __cdecl, LocalizeString, int p_TagIndex, Blam::Text::StringID p_StringId, wchar_t* p_Output)
{
	const static size_t MaxStringLength = 0x400;

	if (p_StringId == 0x0001010A /* start_new_campaign */)
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

DeclareDetouredFunction(Hooks, void, __fastcall, MainMenuUpdate, int thiz, void* unused, Blam::Text::StringID p_MenuName)
{
	//TODO: Struct this out
	auto s_ShouldUpdate = *(uint32_t *)((uint8_t*)thiz + 0x10) >= 0x1E;

	o_MainMenuUpdate(thiz, unused, p_MenuName);

	if (s_ShouldUpdate)
	{
		void *s_UIData = UIImpl::ShowDialog(p_MenuName, 0xFF, 4, 0x1000D);

		if (s_UIData != nullptr)
		*(uint32_t*)((char*)s_UIData + 0x18) = 1;
	}
}