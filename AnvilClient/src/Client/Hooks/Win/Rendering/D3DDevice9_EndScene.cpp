#include <Client/Hooks/WinHooks.hpp>
#include <d3d9.h>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_EndScene, WINAPI, LPDIRECT3DDEVICE9 p_Device)
{
	auto s_Ret = o_D3DDevice9_EndScene(p_Device);

	if (!Rendering::WebRenderer::GetInstance()->Initialized())
	{
		auto s_Success = Rendering::WebRenderer::GetInstance()->InitRenderer(p_Device);
		if (!s_Success)
			WriteLog("WebRenderer Render initialization failed.");

		s_Success = Rendering::WebRenderer::GetInstance()->Init();
		if (!s_Success)
			WriteLog("WebRenderer init failed.");
	}
	else
	{
		Rendering::WebRenderer::GetInstance()->Render(p_Device);
	}

	return s_Ret;
}