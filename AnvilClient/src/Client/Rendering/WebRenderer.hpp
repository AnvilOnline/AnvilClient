#pragma once
#include <Misc/IInit.hpp>
#include <d3d9.h>
#include <d3dx9.h>
#include <include/internal/cef_ptr.h>
#include <include/cef_app.h>
#include "WebRendererClient.hpp"


namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebRendererSchemeHandlerFactory;

			class WebRenderer : public IInit
			{
				//CefRefPtr<CefBrowser> m_Browser;
				CefRefPtr<CefClient> m_Client;
				CefRefPtr<CefRenderHandler> m_RenderHandler;
				CefRefPtr<CefApp> m_App;
				WebRendererSchemeHandlerFactory* m_SchemeHandlerFactory;

				bool m_Initialized;
				bool m_RenderingInitialized;
				bool m_Shutdown;
				bool m_Enabled;

				LPDIRECT3DTEXTURE9 m_Texture;
				LPDIRECT3DDEVICE9 m_Device;
				LPD3DXSPRITE m_Sprite;
				LPD3DXFONT m_Font;

				static WebRenderer* m_Instance;
				WebRenderer();

			protected:
				~WebRenderer()
				{
				}

				

			public:
				static WebRenderer* GetInstance();

				bool Init() override;

				bool InitRenderer(LPDIRECT3DDEVICE9 p_Device);

				bool Render(LPDIRECT3DDEVICE9 p_Device);

				bool Update();

				bool Initialized();

				bool Resize(unsigned long p_Width, unsigned long p_Height);

				bool UpdateMouse(unsigned long p_X, unsigned long p_Y);
				bool Click(unsigned long p_X, unsigned long p_Y);

				bool ExecuteJavascript(std::string p_Code);

				bool PreReset();
				bool PostReset();

				bool Shutdown();

				bool IsEnabled();
				bool Enable(bool p_Enable);

				bool ShowNotification(std::string p_Title, std::string p_Message);

				std::string GetUIDirectory();
			};
		}
	}
}