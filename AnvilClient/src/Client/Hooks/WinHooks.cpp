#include "WinHooks.hpp"

using Anvil::Client::Hooks::WinHooks;

DeclareFunctionValue(WinHooks, CreateFileW);

WinHooks::WinHooks()
{
}

bool WinHooks::Init()
{
	Hook_FileIo();

	WriteLog(L"WinHooks init.");
	return true;
}

void WinHooks::Hook_FileIo()
{
	auto s_KernelBase = GetModuleHandle(L"KernelBase.dll");
	if (!s_KernelBase)
	{
		WriteLog(L"Could not get the handle for kernelbase.dll.");
		return;
	}
		
	auto s_CreateFileW = GetProcAddress(s_KernelBase, "CreateFileW");
	if (MH_CreateHook(s_CreateFileW, (LPVOID*)&c_CreateFileW, (LPVOID*)&o_CreateFileW) != MH_OK)
	{
		WriteLog(L"Could not create CreateFileW hook.");
		return;
	}

	if (MH_EnableHook(s_CreateFileW) != MH_OK)
	{
		WriteLog(L"Could not enable CreateFileW hook.");
		return;
	}

	//DeclareHookAtOffset64(CreateFileW, GetProcAddress(s_KernelBase, "CreateFileW"));

}