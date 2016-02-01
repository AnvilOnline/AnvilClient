#include <Client/Hooks/WinHooks.hpp>
#include <d3d9.h>
#include <Client/Rendering/WebRenderer.hpp>
#include <Client/AnvilClient.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_EndScene, WINAPI, LPDIRECT3DDEVICE9 p_Device)
{
	auto s_Ret = o_D3DDevice9_EndScene(p_Device);

	// Get an instance of our WebRenderer
	auto s_WebRenderer = Rendering::WebRenderer::GetInstance();
	if (!s_WebRenderer)
		return s_Ret;

	// Initialize the renderer 
	if (!s_WebRenderer->Initialized() && AnvilClient::GetInstance()->IsRenderingEnabled())
	{
		// InitRenderer must be called first, this ensures dx is ready to render
		auto s_Success = Rendering::WebRenderer::GetInstance()->InitRenderer(p_Device);
		if (!s_Success)
			WriteLog("WebRenderer Render initialization failed.");

		// Init then starts up CEF and all the web based services
		s_Success = Rendering::WebRenderer::GetInstance()->Init();
		if (!s_Success)
			WriteLog("WebRenderer init failed.");
	}
	else
	{
		// Toggle hiding the UI
		if (GetAsyncKeyState(VK_F5) & 0x8000)
		{
			s_WebRenderer->ShowRenderer(!s_WebRenderer->IsRendering(), false);
			Sleep(250);
		}

		// Check to see if we are set to a "rendering" state to draw everything to the screen
		if (s_WebRenderer->IsRendering())
			s_WebRenderer->Render(p_Device);
	}

	return s_Ret;
}