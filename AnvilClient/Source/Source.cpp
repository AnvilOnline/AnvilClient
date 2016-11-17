#include <Windows.h>
#include <Client/Client.hpp>
#include <Utils/Logger.hpp>
#include <Globals.hpp>

DWORD WINAPI Initialize(LPVOID)
{
	Anvil::Utils::Logger::GetInstance()->Init();

	while (!GetModuleHandle("d3dx9_43.dll"))
		Sleep(250);

	Anvil::Client::AnvilClient::GetInstance()->Init();

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE p_Instance, DWORD p_Reason, LPVOID p_Reserved)
{
	if (p_Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, Initialize, nullptr, 0, nullptr);

	return TRUE;
}