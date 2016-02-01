#include <Client/Hooks/WinHooks.hpp>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_Reset, WINAPI, LPDIRECT3DDEVICE9 p_Device, D3DPRESENT_PARAMETERS* p_PresentParameters)
{
	// Pre-reset function
	Rendering::WebRenderer::GetInstance()->PreReset();

	// Call the game's reset function
	auto s_Ret = o_D3DDevice9_Reset(p_Device, p_PresentParameters);

	// Post-reset function
	Rendering::WebRenderer::GetInstance()->PostReset();

	// Return with a smile
	return s_Ret;
}