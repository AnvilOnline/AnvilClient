#include "WinHooks.hpp"

using Anvil::Client::Hooks::WinHooks;

DeclareFunctionValue(WinHooks, CreateWindowExA);
DeclareFunctionValue(WinHooks, D3DDevice9_EndScene);
DeclareFunctionValue(WinHooks, D3DDevice9_Reset);
DeclareFunctionValue(WinHooks, D3DDevice9_BeginScene);
DeclareFunctionValue(WinHooks, Direct3DCreate9);
DeclareFunctionValue(WinHooks, Direct3D_CreateDevice);

HHOOK WinHooks::m_MouseHook = nullptr;
HHOOK WinHooks::m_WindowHook = nullptr;

WinHooks::WinHooks()
{
}

bool WinHooks::Init()
{
	// CreateWindowEx Hook
	auto s_CreateWindowEx = reinterpret_cast<DWORD>(GetProcAddress(GetModuleHandle("user32.dll"), "CreateWindowExA"));
	DeclareHookAtOffset(CreateWindowExA, s_CreateWindowEx);

	Hook_DirectX();

	WriteLog("WinHooks init success.");

	return true;
}

void WinHooks::Hook_DirectX()
{
	auto s_Library = LoadLibrary("d3d9.dll");
	if (!s_Library)
	{
		WriteLog("Could not load the d3d module.");
		return;
	}

	auto s_Direct3DModule = GetModuleHandle("d3d9.dll");
	if (!s_Direct3DModule)
	{
		WriteLog("Could not get d3d module.");
		return;
	}

	auto s_D3DCreate9 = GetProcAddress(s_Direct3DModule, "Direct3DCreate9");
	if (!s_D3DCreate9)
	{
		WriteLog("Could not get Direct3DCreate9 address.");
		return;
	}

	WriteLog("D3DCreate9 Address: %p", s_D3DCreate9);

	DeclareHookAtOffset(Direct3DCreate9, reinterpret_cast<unsigned long>(s_D3DCreate9));
}