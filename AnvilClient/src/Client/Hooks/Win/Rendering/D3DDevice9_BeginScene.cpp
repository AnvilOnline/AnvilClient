#include <Client/Hooks/WinHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_BeginScene, WINAPI, LPDIRECT3DDEVICE9 p_Device)
{
	auto s_Ret = o_D3DDevice9_BeginScene(p_Device);

	__asm NOP;

	return s_Ret;
}