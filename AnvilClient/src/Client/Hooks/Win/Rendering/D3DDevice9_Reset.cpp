#include <Client/Hooks/WinHooks.hpp>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_Reset, WINAPI, LPDIRECT3DDEVICE9 p_Device, D3DPRESENT_PARAMETERS* p_PresentParameters)
{
	Rendering::WebRenderer::GetInstance()->PreReset();

	auto s_Ret = o_D3DDevice9_Reset(p_Device, p_PresentParameters);

	Rendering::WebRenderer::GetInstance()->PostReset();

	return s_Ret;
}