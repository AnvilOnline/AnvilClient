#pragma once
#include <d3d9.h>
#include "WebUIFrame.hpp"
#include "WebUIApp.hpp"
#include <include/internal/cef_types_wrappers.h>
#include <include/internal/cef_ptr.h>
#include <include/wrapper/cef_message_router.h>
#include "WebUISchemeHandlerFactory.hpp"

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebUI
			{
				CefRefPtr<WebUIApp> m_App;
				bool m_Shutdown;
				bool m_Initialized;
				bool m_HasConsole;

				WebUIFrame* m_Frame;

				bool m_MouseEnabled;
				bool m_KeyboardEnabled;
				bool m_HasDoneIntro;

				HWND m_WindowHandle;
				HINSTANCE m_HandleInstance;
				WNDPROC m_DefaultWndProc;

				WebUISchemeHandlerFactory* m_SchemeHandlerFactory;

				static WebUI* m_Instance;

				WebUI();
				~WebUI();

			public:
				static WebUI* GetInstance();
				static void DestroyInstance();

				void Init(bool p_Debug);
				void Shutdown();
				void Update();
				void Render();

				void CreateFrame(unsigned long p_Width, unsigned long p_Height, LPDIRECT3DDEVICE9 p_Device, CefMessageRouterBrowserSide::Handler* p_Handler);

				void OnMouseEvent(unsigned long p_Width, unsigned long p_Height, unsigned int p_MouseIndex);
				void OnKeyboardEvent(CefKeyEvent p_Event);

				void EnableMouseInput();
				void EnableKeyboardInput();

				void DisableMouseInput();
				void DisableKeyboardInput();

				void InitKeyboardHandler(HINSTANCE p_Instance, HWND p_WindowHandle);

				void ExecuteJavascript(const std::string& p_Code);

				void ToggleConsole();

				__forceinline WebUIFrame* Frame() const { return m_Frame; }
				__forceinline bool Initialized() const { return m_Initialized; }
				__forceinline bool MouseEnabled() const { return m_MouseEnabled; }
				__forceinline bool KeyboardEnabled() const { return m_KeyboardEnabled; }
				__forceinline bool HasConsole() const { return m_HasConsole; }
			};
		}
	}
}