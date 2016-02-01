#include <Client/Hooks/WinHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, Direct3D_CreateDevice, __stdcall, void* Th, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	// Call the original function
	auto s_Ret = o_Direct3D_CreateDevice(Th, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	if (s_Ret != D3D_OK)
	{
		WriteLog("There was an error in CreateDevice! Returned: %x, skipping dx hooking.", s_Ret);
		return s_Ret;
	}

	// Get the created device
	auto s_Device = *ppReturnedDeviceInterface;
	if (!s_Device)
	{
		WriteLog("The returned d3d device is invalid. Result: %x", s_Ret);
		return s_Ret;
	}

	// Get the VTable
	auto pVTable = reinterpret_cast<uint32_t*>(s_Device);
	pVTable = reinterpret_cast<uint32_t*>(pVTable[0]);

	// Get all of the addresses to hook by vtable
	auto s_BeginScene = pVTable[41];
	auto s_EndScene = pVTable[42];
	auto s_Reset = pVTable[16];

	DeclareHookAtOffset(D3DDevice9_BeginScene, s_BeginScene);
	DeclareHookAtOffset(D3DDevice9_EndScene, s_EndScene);
	DeclareHookAtOffset(D3DDevice9_Reset, s_Reset);

	WriteLog("CreateDevice called, DirectX functions hooked.");

	return s_Ret;
}