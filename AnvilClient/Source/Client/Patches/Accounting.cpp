#include <Client/Patch.hpp>
#include <Utils/Util.hpp>

using namespace Anvil::Client;

void AnvilPatch::Patch_Accounting()
{
	// No "--account" argument patch
	Utils::Util::PatchAddressInFile(0x43731A, "\xEB\x0E", 2);
	Utils::Util::PatchAddressInFile(0x4373AD, "\xEB\x03", 2);
}