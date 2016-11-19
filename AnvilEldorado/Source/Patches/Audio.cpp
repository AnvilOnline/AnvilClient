#include "Patch.hpp"
#include <Utils/Util.hpp>

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

__declspec(naked) void FmodSystemInitHook()
{
	__asm
	{
		push	0; extradriverdata
		push	ebx; flags
		push	100h; maxchannels
		push	eax; FMOD_SYSTEM
		call	dword ptr[ecx]; FMOD::System::init
		push	0x404EA4
		ret
	}
}

__declspec(naked) void FmodSystemInitHook2()
{
	__asm
	{
		push	0; extradriverdata
		push	ebx; flags
		push	100h; maxchannels
		push	eax; FMOD_SYSTEM
		call	dword ptr[ecx]; FMOD::System::init
		push	0x404EC8
		ret
	}
}

void AnvilPatch::Patch_Audio()
{
	// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
	// TODO: maybe find a way to update HO's FMOD, HO is using 4.26.6 which is ancient
	Utils::Util::PatchAddressInFile(0x100DA75, "\x02", 1);

	// increase max virtual audio channels from 64 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
	Utils::Util::ApplyHook(0x4E9C, FmodSystemInitHook);
	Utils::Util::ApplyHook(0x4EC0, FmodSystemInitHook2);

	// increase software channels from 192 to 256
	// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
	Utils::Util::PatchAddressInMemory(reinterpret_cast<void *>(0x404DF8 + 1), "\x00\x00\x01\x00", 4);
}