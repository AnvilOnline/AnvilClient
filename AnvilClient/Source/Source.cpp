#include <Windows.h>
#include <Client/Client.hpp>

#include <Utils/Logger.hpp>

DWORD WINAPI Initialize(LPVOID)
{
	AnvilCommon::Utils::Logger::GetInstance()->Init();

	Anvil::Client::AnvilClient::GetInstance()->Init();

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE p_Instance, DWORD p_Reason, LPVOID p_Reserved)
{
	if (p_Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, Initialize, nullptr, 0, nullptr);

	return TRUE;
}