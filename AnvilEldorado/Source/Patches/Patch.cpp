#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

/*
BIG FUCKING TODO:
ENSURE THAT THESE OFFSETS ARE CORRECT
AND THAT THEY ARE USING THE RIGHT INFILE/INMEMORY FUNCTIONS!!!!!!
*/

bool AnvilPatch::Init()
{
	// Remove Preferences.dat hash check
	Utils::Util::PatchAddressInFile(0x10C99A, "\x90\x90\x90\x90\x90\x90", 6);

	// Allow spawning AI through effects
	Utils::Util::PatchAddressInFile(0x1033321, "\x90\x90", 2);

	// Fix random colored lighting
	Utils::Util::PatchAddressInFile(0x14F2FFC, "\x00\x00\x00\x00", 4);

	return false;
}