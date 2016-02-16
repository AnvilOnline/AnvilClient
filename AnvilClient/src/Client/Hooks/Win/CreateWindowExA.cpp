#include <Client/Hooks/WinHooks.hpp>
#include <Windows.h>
#include <Client/Rendering/WebRenderer.hpp>
#include <Client/AnvilClient.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(WinHooks, HWND, CreateWindowExA, __stdcall, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hwndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	// Get our current build information
	auto s_Version = AnvilClient::GetInstance()->GetVersion();

	// Call the original CreateWindowExA function
	auto s_Ret = o_CreateWindowExA(dwExStyle, lpClassName, s_Version.c_str(), dwStyle, X, Y, nWidth, nHeight, hwndParent, hMenu, hInstance, lpParam);

	// We set the function here to ensure we can use it later.
	AnvilClient::GetInstance()->SetWindowHandle(s_Ret);

	// Get the process thread id
	auto s_ThreadId = GetWindowThreadProcessId(s_Ret, nullptr);

	// Install the mouse and window hooks
	m_MouseHook = SetWindowsHookEx(WH_MOUSE, HookMouseProc, nullptr, s_ThreadId);
	if (!m_MouseHook)
		WriteLog("Mouse hook failed (%x).", GetLastError());

	m_WindowHook = SetWindowsHookEx(WH_CALLWNDPROCRET, HookWindowProc, nullptr, s_ThreadId);
	if (!m_WindowHook)
		WriteLog("Window hook failed (%x).", GetLastError());

	// Call resize on the renderer to prepare the buffers.
	Rendering::WebRenderer::GetInstance()->Resize(nWidth, nHeight);

	return s_Ret;
}
