#include "Utils\Util.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	__declspec(naked) void FmodSystemInitHook()
	{
		__asm
		{
			push	0; extradriverdata
			push	ebx; flags
			push	100h; maxchannels
			push	eax; FMOD_SYSTEM
			call	dword ptr[ecx] // FMOD::System::init
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
			call	dword ptr[ecx] // FMOD::System::init
			push	0x404EC8
			ret
		}
	}

	bool Engine::ApplyPatches_Audio()
	{
		using AnvilCommon::Utils::Util;

			// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
		return Util::PatchAddress(0x100DA75, "\x02", 1)
			// Increase max virtual audio channels from 64 to 256
			// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
			&& Util::ApplyHook(0x4E9C, FmodSystemInitHook)
			&& Util::ApplyHook(0x4EC0, FmodSystemInitHook2)
			// Increase software channels from 192 to 256
			// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
			&& Util::PatchAddress(0x4DF9, "\x00\x00\x01\x00", 4);
	}
}