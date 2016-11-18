#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

void AnvilPatch::Patch_Tags()
{
	// Enable Tag Editing
	Utils::Util::PatchAddressInFile(0x101A5B, "\xEB", 1);
	Utils::Util::PatchAddressInFile(0x102874, "\x90\x90", 2);
	Utils::Util::PatchAddressInFile(0x1030AA, "\x90\x90", 2);
}