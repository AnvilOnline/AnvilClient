#include <Client/Hooks/WinHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, Direct3D_CreateDevice, __stdcall, void* Th, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	auto s_Ret = o_Direct3D_CreateDevice(Th, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	auto s_Device = *ppReturnedDeviceInterface;

	auto pVTable = reinterpret_cast<DWORD*>(s_Device);
	pVTable = reinterpret_cast<DWORD*>(pVTable[0]);

	auto s_BeginScene = pVTable[41];
	auto s_EndScene = pVTable[42];
	auto s_Reset = pVTable[16];

	DeclareHookAtOffset(D3DDevice9_BeginScene, s_BeginScene);
	DeclareHookAtOffset(D3DDevice9_EndScene, s_EndScene);
	DeclareHookAtOffset(D3DDevice9_Reset, s_Reset);

	WriteLog("CreateDevice called, DirectX functions hooked.");

	return s_Ret;
}