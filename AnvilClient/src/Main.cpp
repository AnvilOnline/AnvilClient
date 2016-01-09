#include <Client/AnvilClient.hpp>
#include <Utils/Logger.hpp>
#include <Windows.h>
#include "Utils/Util.hpp"

uint32_t __stdcall Initialize(LPVOID)
{
	// Init our logger
	Anvil::Utils::Logger::GetInstance()->Init();

#ifdef _DEBUG
	WriteLog("Command Line Arguments: %s.", GetCommandLine());
#endif

	// Wait for DirectX 9 to get loaded
	while (!GetModuleHandle("d3dx9_43.dll"))
		Sleep(250);

	// Initialize the Anvil Client.
	Anvil::Client::AnvilClient::GetInstance()->Init();

	// This is some superhacks right here.
	// This kick-starts all threads that have been created if they haven't already (aka this thread should be the only one running.)
	Anvil::Utils::Util::ResumeAllThreads();

	return S_OK;
}

bool __stdcall DllMain(void* p_Module, uint32_t p_Reason, void* p_Reserved)
{
	if (p_Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialize), nullptr, 0, nullptr);

	return true;
}