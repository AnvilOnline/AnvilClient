#include "Hooks.hpp"
#include <Client\ClientImpl.hpp>
#include <Interfaces\Engine.hpp>

using namespace AnvilEldorado::Game::Forge;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	auto s_Address = s_Base + 0x19F0E0;
	HookFunctionOffset(s_Address, Forge_UpdateInput);

	s_Address = s_Base + 0x28C410;
	HookFunctionOffset(s_Address, UpdateKillTriggers);

	s_Address = s_Base + 0xEB130;
	HookFunctionOffset(s_Address, CheckSafeZones);

	s_Address = s_Base + 0xE97D0;
	HookFunctionOffset(s_Address, PushBarriersGetStructureDesign);

	// TODO: Figure out what these do, and if we can install proper hooks for these
	//		return Patch(0x19F17F, 0x75).Apply()
	//			&& Patch::NopFill(0x19F198, 4)
	return true;
}

int8_t __stdcall Hooks::hk_Forge_UpdateInput(void* a1)
{
	return o_Forge_UpdateInput(a1);
}

bool __cdecl Hooks::hk_UpdateKillTriggers(void* a1, void* a2)
{
	return o_UpdateKillTriggers(a1, a2);
}

void* __cdecl Hooks::hk_PushBarriersGetStructureDesign(void* a1)
{
	return o_PushBarriersGetStructureDesign(a1);
}

Hooks::Forge_UpdateInput_t Hooks::o_Forge_UpdateInput = nullptr;
Hooks::UpdateKillTriggers_t Hooks::o_UpdateKillTriggers = nullptr;
Hooks::CheckSafeZones_t Hooks::o_CheckSafeZones = nullptr;
Hooks::PushBarriersGetStructureDesign_t Hooks::o_PushBarriersGetStructureDesign = nullptr;