#include <Client/Patch.hpp>
#include <Utils/Util.hpp>

using namespace Anvil::Client;

void AnvilPatch::Patch_Equipment()
{
	// Enable Tag Editing
	Utils::Util::PatchAddressInFile(0x786CFF, "\x90\x90\x90\x90\x90\x90", 6);
	Utils::Util::PatchAddressInFile(0x786CF7, "\x90\x90\x90\x90\x90\x90", 6);

	// Prevent game variant weapons from being overridden
	Utils::Util::PatchAddressInFile(0x1A315F, "\xEB", 1);
	Utils::Util::PatchAddressInFile(0x1A31A4, "\xEB", 1);
}