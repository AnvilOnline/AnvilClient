#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

void AnvilPatch::Patch_Rendering()
{
	// Disable converting the game's resolution to 16:9
	Utils::Util::PatchAddressInFile(0x62217D, "\x90\x90", 2);
	Utils::Util::PatchAddressInFile(0x622183, "\x90\x90\x90\x90\x90\x90", 6);

	// Allow the user to select any resolution that Windows supports in the settings screen.
	Utils::Util::PatchAddressInFile(0x10BF1B, "\x90\x90", 2);
	Utils::Util::PatchAddressInFile(0x10BF21, "\x90\x90\x90\x90\x90\x90", 6);
}