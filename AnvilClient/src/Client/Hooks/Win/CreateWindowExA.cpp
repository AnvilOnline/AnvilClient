#include <Client/Hooks/WinHooks.hpp>
#include <Windows.h>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HWND, CreateWindowExA, __stdcall, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hwndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	std::string s_version("AnvilOnline Client Version: ");
	s_version += "alpha";

	auto s_ret = o_CreateWindowExA(dwExStyle, lpClassName, s_version.c_str(), dwStyle, X, Y, nWidth, nHeight, hwndParent, hMenu, hInstance, lpParam);
	auto s_threadId = GetWindowThreadProcessId(s_ret, nullptr);

	// TODO: Implement
	/*m_MouseHook = SetWindowsHookEx(WH_MOUSE, HookMouseProc, nullptr, s_threadId);
	if (!m_MouseHook)
		WriteLog("Mouse hook failed (%x).", GetLastError());

	m_WindowHook = SetWindowsHookEx(WH_CALLWNDPROCRET, HookWindowProc, nullptr, s_threadId);
	if (!m_WindowHook)
		WriteLog("Window hook failed (%x).", GetLastError());*/

	return s_ret;
}
