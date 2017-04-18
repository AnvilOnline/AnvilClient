#include "Hooks.hpp"
#include <Interfaces\Client.hpp>
#include <Interfaces\Engine.hpp>

using namespace AnvilEldorado::Game::Audio;

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

DeclareDetouredFunction(Hooks, void*, __stdcall, FMOD_System_Init, void* a1, int maxChannels, void* a3, void* a4, void* a5)
{
	WriteLog("Max channels changed from %d to %d", maxChannels, 256);

	return o_FMOD_System_Init(a1, 256, a3, a4, a5);
}

DeclareDetouredFunction(Hooks, void*, __stdcall, FMOD_System_SetSoftwareChannels, void* system, int numsoftwarechannels)
{
	WriteLog("Max software channels changed from %d to %d.", numsoftwarechannels, 256);

	return o_FMOD_System_SetSoftwareChannels(system, 256);
}