#include "Hooks.hpp"
#include <Client\ClientImpl.hpp>
#include <Interfaces\Engine.hpp>

using namespace AnvilEldorado::Audio;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	// Hook FMOD::System::Init
	auto s_Address = s_Base + 0xF69AA0;
	HookFunctionOffset(s_Address, FMOD_System_Init);

	// Hook FMOD::System::SetSoftwareChannels
	s_Address = s_Base + 0xF88880;
	HookFunctionOffset(s_Address, FMOD_System_SetSoftwareChannels);

	/*
		kiwidog notes -
		audioses.dll = 1001A1F0
		audioses + 1A1F0
		takes 3 arguments
		last is the 0, that we need to change to a 1

		Don't hardcode this, try and attempt to find where/how to patch properly...This may be the only patch hopefully
	*/

	return true;
}

void* __stdcall Hooks::hk_FMOD_System_Init(void* a1, int maxChannels, void* a3, void* a4, void* a5)
{
	WriteLog("Max channels changed from %d to %d", maxChannels, 256);

	return o_FMOD_System_Init(a1, 256, a3, a4, a5);
}

void* __stdcall Hooks::hk_FMOD_System_SetSoftwareChannels(void* system, int numsoftwarechannels)
{
	WriteLog("Max software channels changed from %d to %d.", numsoftwarechannels, 256);

	return o_FMOD_System_SetSoftwareChannels(system, 256);
}

Hooks::FMOD_System_Init_t Hooks::o_FMOD_System_Init = nullptr;
Hooks::FMOD_System_SetSoftwareChannels_t Hooks::o_FMOD_System_SetSoftwareChannels = nullptr;

//#include "Utils\Hook.hpp"
//#include "Utils\Patch.hpp"
//
//#include "Audio.hpp"
//
//namespace AnvilEldorado::Audio
//{
//	__declspec(naked) void FmodSystemInitHook()
//	{
//		__asm
//		{
//			push	0; extradriverdata
//			push	ebx; flags
//			push	100h; maxchannels
//			push	eax; FMOD_SYSTEM
//			call	dword ptr[ecx] // FMOD::System::init
//			push	0x404EA4
//			ret
//		}
//	}
//
//	__declspec(naked) void FmodSystemInitHook2()
//	{
//		__asm
//		{
//			push	0; extradriverdata
//			push	ebx; flags
//			push	100h; maxchannels
//			push	eax; FMOD_SYSTEM
//			call	dword ptr[ecx] // FMOD::System::init
//			push	0x404EC8
//			ret
//		}
//	}
//
//	bool Audio::Init()
//	{
//		using AnvilCommon::Utils::Hook;
//		using AnvilCommon::Utils::Patch;
//
//			// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
//		return Patch(0x100DA75, 0x02).Apply()
//			// Increase max virtual audio channels from 64 to 256
//			// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
//			&& Hook(0x4E9C, FmodSystemInitHook).Apply()
//			&& Hook(0x4EC0, FmodSystemInitHook2).Apply()
//			// Increase software channels from 192 to 256
//			// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
//			&& Patch(0x4DF9, { 0x00, 0x00, 0x01, 0x00 }).Apply();
//	}
//}