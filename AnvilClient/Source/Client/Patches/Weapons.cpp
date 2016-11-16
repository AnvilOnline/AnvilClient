#include <Client/Patch.hpp>
#include <Utils/Util.hpp>

using namespace Anvil::Client;

void AnvilPatch::Patch_Weapons()
{
	// Force descoping for all weapons
	Utils::Util::PatchAddressInFile(0x73F1E6, "\x00", 1);
}