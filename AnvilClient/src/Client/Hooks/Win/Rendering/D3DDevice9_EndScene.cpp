#include <Client/Hooks/WinHooks.hpp>
#include <d3d9.h>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_EndScene, WINAPI, LPDIRECT3DDEVICE9 p_Device)
{
	auto s_Ret = o_D3DDevice9_EndScene(p_Device);

	//p_Device->Clear(1, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 128, 128, 128), 0, 0);

	return s_Ret;
}