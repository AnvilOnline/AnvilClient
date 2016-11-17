#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

void AnvilPatch::Patch_Equipment()
{
	// Enable Tag Editing
	Utils::Util::PatchAddressInFile(0xB86CFF, "\x90\x90\x90\x90\x90\x90", 6);
	Utils::Util::PatchAddressInFile(0xB86CF7, "\x90\x90\x90\x90\x90\x90", 6);

	// Prevent game variant weapons from being overridden
	Utils::Util::PatchAddressInFile(0x5A315F, "\xEB", 1);
	Utils::Util::PatchAddressInFile(0x5A31A4, "\xEB", 1);
}