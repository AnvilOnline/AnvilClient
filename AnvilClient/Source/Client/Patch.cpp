#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace Anvil::Client;

/*
	BIG FUCKING TODO:
	ENSURE THAT THESE OFFSETS ARE CORRECT
	AND THAT THEY ARE USING THE RIGHT INFILE/INMEMORY FUNCTIONS!!!!!!
*/

bool AnvilPatch::PreInit()
{
	// Patch all required functions before the engine starts up

	// Remove Preferences.dat hash check
	Utils::Util::PatchAddressInFile(0x10C99A, "\x90\x90\x90\x90\x90\x90", 6);

	return false;
}

bool AnvilPatch::Init()
{
	// Patch all required functions on-engine startup

	// Allow spawning AI through effects
	Utils::Util::PatchAddressInFile(0x1033321, "\x90\x90", 2);

	// Fix random colored lighting
	Utils::Util::PatchAddressInFile(0x14F2FFC, "\x00\x00\x00\x00", 4);

	return false;
}

bool AnvilPatch::PostInit()
{
	// Patch anything that needs to be done post-engine startup
	return false;
}
