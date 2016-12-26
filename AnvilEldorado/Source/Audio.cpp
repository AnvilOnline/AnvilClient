#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
#include "Audio.hpp"

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

	bool Audio::Init()
	{
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
		return Patch(0x100DA75, 0x02).Apply()
			// Increase max virtual audio channels from 64 to 256
			// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
			&& Hook(0x4E9C, FmodSystemInitHook).Apply()
			&& Hook(0x4EC0, FmodSystemInitHook2).Apply()
			// Increase software channels from 192 to 256
			// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
			&& Patch(0x4DF9, { 0x00, 0x00, 0x01, 0x00 }).Apply();
	}
}