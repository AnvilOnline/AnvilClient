#include <Client/Hooks/WinHooks.hpp>
#include <Windows.h>

using namespace Anvil::Client::Hooks;

// TODO: See if this is still needed at all...
LRESULT WinHooks::HookWindowProc(int p_Code, WPARAM p_WParam, LPARAM p_LParam)
{
	if (p_Code < HC_ACTION)
		return CallNextHookEx(WinHooks::m_WindowHook, p_Code, p_WParam, p_LParam);

	auto s_Message = reinterpret_cast<MSG*>(p_LParam);
	if (s_Message->message == WM_CLOSE || s_Message->message == WM_QUIT)
	{
		WriteLog("Close or Quit called.");
	}

	return CallNextHookEx(WinHooks::m_WindowHook, p_Code, p_WParam, p_LParam);
}