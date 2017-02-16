#include <Windows.h>
#include <Client/Client.hpp>

#include <Utils/Logger.hpp>

DWORD WINAPI Initialize(LPVOID)
{
	// Initialize the logger before anything else
	AnvilCommon::Utils::Logger::Instance()->Init();

	// Initialize the client, which will in turn initialize Eldorado or Ausar engine's
	Anvil::Client::AnvilClient::Instance()->Init();

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE p_Instance, DWORD p_Reason, LPVOID p_Reserved)
{
	if (p_Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, Initialize, nullptr, 0, nullptr);

	return TRUE;
}