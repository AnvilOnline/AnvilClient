#include <Client/Hooks/WinHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, void*, Direct3DCreate9, __stdcall, uint32_t p_SdkVersion)
{
	// Call the original function
	auto s_IDirect3D9 = static_cast<LPDIRECT3D9>(o_Direct3DCreate9(p_SdkVersion));
	
	WriteLog("Direct3DCreate9 called.");

	// Get the VTable for IDirect3D9
	auto pVTable = reinterpret_cast<DWORD*>(s_IDirect3D9);
	pVTable = reinterpret_cast<DWORD*>(pVTable[0]);

	// Get CreateDevice function off of the VTable
	auto s_CreateDevice = pVTable[16];

	WriteLog("CreateDevice: %p", s_CreateDevice);

	// Install CreateDevice hook
	DeclareHookAtOffset(Direct3D_CreateDevice, s_CreateDevice);

	WriteLog("DirectX Init Hooks installed.");

	// Continue execution like ~~nothing~~ happened
	return s_IDirect3D9;
}
