#include "Hooks.hpp"
#include <Client\ClientImpl.hpp>
#include <Interfaces\Engine.hpp>

using namespace AnvilEldorado::Game;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	// Initialize Hooks
	// Saber initialize
	auto s_Address = s_Base + 0x699120;
	HookFunctionOffset(s_Address, Game_InitHalo3);

	// Hook tags loaded notification
	s_Address = s_Base + 0x1017E0;
	HookFunctionOffset(s_Address, Game_TagsLoaded);

	return true;
}

void* Hooks::hk_Game_InitHalo3()
{
	const auto Game_InitAudioSystem = reinterpret_cast<bool(*)()>(0x64E190);

	// Initialize Audio
	Game_InitAudioSystem();

	WriteLog("Game::InitHalo3 called.");

	return nullptr;
}

int __cdecl Hooks::hk_Game_TagsLoaded(char* p_TagType)
{
	WriteLog("Game::TagsLoaded called.");

	return o_Game_TagsLoaded(p_TagType);
}

void __fastcall Hooks::hk_sub_591290(void* th)
{
	WriteLog("ESP: %p.", th);

	o_sub_591290(th);

	WriteLog("ESP2: %p.", th);
}

Hooks::Game_VerifyMapSignature_t Hooks::o_Game_VerifyMapSignature = nullptr;
Hooks::Game_InitHalo3_t Hooks::o_Game_InitHalo3 = nullptr;
Hooks::Game_TagsLoaded_t Hooks::o_Game_TagsLoaded = nullptr;
Hooks::sub_591290_t Hooks::o_sub_591290 = nullptr;