#include <Client/Hooks/WinHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HANDLE, CreateFileW, WINAPI, LPCWSTR lpFilename, DWORD dwAccess, DWORD dwSharing, LPSECURITY_ATTRIBUTES saAttributes, DWORD dwCreation, DWORD dwAttributes, HANDLE hTemplate)
{
	auto s_Ret = o_CreateFileW(lpFilename, dwAccess, dwSharing, saAttributes, dwCreation, dwAttributes, hTemplate);

	WriteLog(L"Got a CreateFileW request: %s %x.", lpFilename, s_Ret);

	return s_Ret;
}