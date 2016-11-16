#include <Client/Patch.hpp>
#include <Utils/Util.hpp>

using namespace Anvil::Client;

void AnvilPatch::Patch_Camera()
{
	// Prevent FOV from being overridden when the game loads
	Utils::Util::PatchAddressInFile(0x25FA79, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);
	Utils::Util::PatchAddressInFile(0x25FA86, "\x90\x90\x90\x90\x90", 6);
}