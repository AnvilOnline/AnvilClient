#include "Hooks.hpp"
#include "PlayerImpl.hpp"

#include <Interfaces\Client.hpp>
#include <EngineImpl.hpp>

using namespace AnvilEldorado::Game::Players;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	auto s_Address = s_Base + 0x3B7B70;
	HookFunctionOffset(s_Address, SetupPlayerArmor);

	return true;
}

DeclareDetouredFunction(Hooks, void, __cdecl, SetupPlayerArmor, double a1, __m128 a2)
{
	auto s_Player = GetClientInterface()->GetEngine()->GetSubsystem<PlayerImpl>();

	// This function runs every tick, so only update if necessary
	if (!s_Player->NeedsArmorUpdate())
		return;

	// Try to get the UI player biped
	auto s_UiPlayerBiped = s_Player->GetPodiumBiped();
	if (s_UiPlayerBiped == Blam::Data::DatumIndex::Null)
		return;

	s_Player->CustomizeBiped(s_UiPlayerBiped);
	s_Player->SetUpdateArmor(false);
}