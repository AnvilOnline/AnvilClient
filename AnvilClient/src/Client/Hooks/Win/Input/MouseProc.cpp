#include <Windows.h>
#include <Client/Hooks/WinHooks.hpp>
#include <Client/Rendering/WebRenderer.hpp>

#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
using namespace Anvil::Client::Hooks;

LRESULT WinHooks::HookMouseProc(int p_Code, WPARAM p_WParam, LPARAM p_LParam)
{
	if (p_Code < HC_ACTION)
		return CallNextHookEx(m_MouseHook, p_Code, p_WParam, p_LParam);

	if (p_WParam == WM_MOUSEMOVE)
	{
		auto s_Struct = reinterpret_cast<LPMOUSEHOOKSTRUCT>(p_LParam);

		if (!Rendering::WebRenderer::GetInstance()->UpdateMouse(s_Struct->pt.x, s_Struct->pt.y))
			return CallNextHookEx(m_MouseHook, p_Code, p_WParam, p_LParam);
	}

	if (p_WParam == WM_LBUTTONDOWN)
	{
		auto s_Struct = reinterpret_cast<LPMOUSEHOOKSTRUCT>(p_LParam);

		auto s_X = s_Struct->pt.x;
		auto s_Y = s_Struct->pt.y;

		if (!Rendering::WebRenderer::GetInstance()->Click(s_X, s_Y))
			return CallNextHookEx(m_MouseHook, p_Code, p_WParam, p_LParam);
	}

	return CallNextHookEx(m_MouseHook, p_Code, p_WParam, p_LParam);
}
