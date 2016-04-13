#include <Client/Hooks/WinHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_SetViewport, WINAPI, LPDIRECT3DDEVICE9 p_Device, D3DVIEWPORT9* p_Viewport)
{
	if (p_Viewport)
		p_Viewport->X = 0;

	return  o_D3DDevice9_SetViewport(p_Device, p_Viewport);
}