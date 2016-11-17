#include <Hooks/Hook.hpp>
#include <Windows.h>
#include <detours.h>

#include <Utils/Logger.hpp>

using namespace AnvilEldorado::Hooks;

Hook::EndScene_t Hook::EndScene = nullptr;

void Hook::Hook_DirectX()
{
	auto s_Library = LoadLibrary("d3d9.dll");
	if (!s_Library)
	{
		WriteLog("Could not load the Direct3D module.");
		return;
	}

	auto s_Direct3DModule = GetModuleHandle("d3d9.dll");
	if (!s_Direct3DModule)
	{
		WriteLog("Could not get Direct3D module.");
		return;
	}

	auto s_D3DCreate9 = GetProcAddress(s_Direct3DModule, "Direct3DCreate9");
	if (!s_D3DCreate9)
	{
		WriteLog("Could not get Direct3DCreate9 address.");
		return;
	}

	WriteLog("D3DCreate9 Address: %p", s_D3DCreate9);

	EndScene = reinterpret_cast<EndScene_t>(DetourFunction(reinterpret_cast<PBYTE>(s_D3DCreate9), reinterpret_cast<PBYTE>(c_EndScene)));
}

uint32_t __stdcall Hook::c_EndScene(void* p_Device)
{
	__asm NOP;

	WriteLog("EndScene called.");

	return EndScene(p_Device);
}