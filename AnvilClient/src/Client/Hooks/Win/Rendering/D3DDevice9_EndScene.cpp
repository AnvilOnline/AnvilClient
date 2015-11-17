#include <Client/Hooks/WinHooks.hpp>
#include <d3d9.h>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_EndScene, WINAPI, LPDIRECT3DDEVICE9 p_Device)
{
	auto s_Ret = o_D3DDevice9_EndScene(p_Device);

	auto s_WebRenderer = Rendering::WebRenderer::GetInstance();

	if (!s_WebRenderer->Initialized())
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
		if (GetAsyncKeyState(VK_F5) & 0x8000)
			s_WebRenderer->Enable(!s_WebRenderer->IsEnabled());

		if (s_WebRenderer->IsEnabled())
			s_WebRenderer->Render(p_Device);
	}

	return s_Ret;
}