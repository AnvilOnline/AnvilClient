#include <Client/Patch.hpp>
#include <Utils/Util.hpp>

using namespace Anvil::Client;

void AnvilPatch::Patch_Audio()
{
	// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
	// TODO: maybe find a way to update HO's FMOD, HO is using 4.26.6 which is ancient
	Utils::Util::PatchAddressInFile(0x100DA75, "\x02", 1);

	// increase software channels from 192 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
	*reinterpret_cast<uint32_t*>(0x404DF8 + 1) = 256;
}