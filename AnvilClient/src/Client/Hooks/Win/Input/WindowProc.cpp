#include <Client/Hooks/WinHooks.hpp>
#include <Windows.h>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

LRESULT WinHooks::HookWindowProc(int p_Code, WPARAM p_WParam, LPARAM p_LParam)
{
	if (p_Code < HC_ACTION)
		return CallNextHookEx(WinHooks::m_WindowHook, p_Code, p_WParam, p_LParam);

	auto s_Struct = reinterpret_cast<CWPSTRUCT*>(p_LParam);
	if (!s_Struct)
		return CallNextHookEx(WinHooks::m_WindowHook, p_Code, p_WParam, p_LParam);

	Rendering::WebRenderer::GetInstance()->Update();

	return CallNextHookEx(WinHooks::m_WindowHook, p_Code, p_WParam, p_LParam);
}