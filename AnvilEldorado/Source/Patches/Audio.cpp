#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

void AnvilPatch::Patch_Audio()
{
	// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
	// TODO: maybe find a way to update HO's FMOD, HO is using 4.26.6 which is ancient
	Utils::Util::PatchAddressInFile(0x100DA75, "\x02", 1);

	// increase software channels from 192 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
	Utils::Util::PatchAddressInMemory(reinterpret_cast<void *>(0x404DF8 + 1), "\x00\x00\x01\x00", 4);
}