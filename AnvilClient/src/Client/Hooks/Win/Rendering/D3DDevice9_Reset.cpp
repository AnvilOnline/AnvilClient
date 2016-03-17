#include <Client/Hooks/WinHooks.hpp>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_Reset, WINAPI, LPDIRECT3DDEVICE9 p_Device, D3DPRESENT_PARAMETERS* p_PresentParameters)
{
	auto s_Renderer = Rendering::WebRenderer::GetInstance();

	// Pre-reset function
	if (!s_Renderer->PreReset())
		WriteLog("PreReset failed.");

	// Call the game's reset function
	auto s_Ret = o_D3DDevice9_Reset(p_Device, p_PresentParameters);

	// Post-reset function
	if (!s_Renderer->PostReset())
		WriteLog("PostReset failed.");

	if (s_Renderer->Resize(p_PresentParameters->BackBufferWidth, p_PresentParameters->BackBufferHeight))
		WriteLog("Resize really failed.");

	// Return with a smile
	return s_Ret;
}