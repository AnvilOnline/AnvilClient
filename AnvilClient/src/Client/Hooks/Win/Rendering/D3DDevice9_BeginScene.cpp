#include <Client/Hooks/WinHooks.hpp>
#include <Client/Rendering/WebUI.hpp>
#include <Client/Rendering/Handlers/MainHandler.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HRESULT, D3DDevice9_BeginScene, WINAPI, LPDIRECT3DDEVICE9 p_Device)
{
	auto s_Ret = o_D3DDevice9_BeginScene(p_Device);

	__asm NOP;

	D3DVIEWPORT9 s_Viewport;
	p_Device->GetViewport(&s_Viewport);

	if (!Rendering::WebUI::GetInstance()->Initialized())
		Rendering::WebUI::GetInstance()->CreateFrame(s_Viewport.Width, s_Viewport.Height, p_Device, new Rendering::Handlers::MainHandler());
	else
		Rendering::WebUI::GetInstance()->Update();

	return s_Ret;
}