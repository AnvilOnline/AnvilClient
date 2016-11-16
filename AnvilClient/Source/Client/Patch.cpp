#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace Anvil::Client;

bool AnvilPatch::PreInit()
{
	// Patch all required functions before the engine starts up

	// Remove Preferences.dat hash check
	Utils::Util::PatchAddressInFile(0x10C99A, "\x90\x90\x90\x90\x90\x90", 6);

	// Allow spawning AI through effects
	Utils::Util::PatchAddressInFile(0x1033321, "\x90\x90", 2);

	return false;
}

bool AnvilPatch::Init()
{
	// Patch all required functions on-engine startup
	return false;
}

bool AnvilPatch::PostInit()
{
	// Patch anything that needs to be done post-engine startup
	return false;
}
