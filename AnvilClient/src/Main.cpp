#include <Client/AnvilClient.hpp>
#include <Utils/Logger.hpp>
#include <Misc/UwpHelper.hpp>

#include <Windows.h>

uint32_t __stdcall Initialize(LPVOID)
{
	// Init UwpHelper
	Anvil::UwpHelper::GetInstance()->Init();

	// Init our logger
	Anvil::Utils::Logger::GetInstance()->Init();

#ifdef _DEBUG
	WriteLog(L"Command Line Arguments: %s.", GetCommandLine());
#endif

	// Wait for DirectX 12 to get loaded
	/*while (!GetModuleHandle(L"d3d12.dll"))
		Sleep(250);*/

	// Initialize the Anvil Client.
	Anvil::Client::AnvilClient::GetInstance()->Init();

	return S_OK;
}

bool __stdcall DllMain(void* p_Module, uint32_t p_Reason, void* p_Reserved)
{
	if (p_Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialize), nullptr, 0, nullptr);

	return true;
}