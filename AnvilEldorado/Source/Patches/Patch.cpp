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
	Utils::Util::PatchAddressInFile(0x50C99A, "\x90\x90\x90\x90\x90\x90", 6);

	// Allow spawning AI through effects
	Utils::Util::PatchAddressInFile(0x1433321, "\x90\x90", 2);

	// Fix random colored lighting
	Utils::Util::PatchAddressInFile(0x18F2FFC, "\x00\x00\x00\x00", 4);
	
	Patch_Tags();
	Patch_Accounting();
	Patch_Camera();
	Patch_Weapons();
	Patch_Audio();
	Patch_Equipment();
	Patch_Rendering();

	return true;
}