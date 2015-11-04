#pragma once
#include <include/wrapper/cef_message_router.h>
#include "WebUIClient.hpp"
#include <d3d9.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebUIFrame
			{
			public:
				WebUIFrame(bool p_Internal, unsigned long p_Width, unsigned long p_Height, CefMessageRouterBrowserSide::Handler* p_Handler);
				~WebUIFrame();

				bool Init(const std::string& p_URL, LPDIRECT3DDEVICE9 p_Device);
				void Update();
				void Render();
				void OnResized(unsigned long p_Width, unsigned long p_Height);
				void ExecuteJavascript(const std::string& p_Code);

				__forceinline unsigned long FrameWidth() const { return m_Width; }
				__forceinline unsigned long FrameHeight() const { return m_Height; }

				__forceinline LPDIRECT3DTEXTURE9 MainTexture() const { return m_MainTexture; }

				__forceinline void ShouldRender(bool p_Render) { m_ShouldRender = p_Render; }
				__forceinline bool ShouldRender() const { return m_ShouldRender; }

				__forceinline CefRefPtr<WebUIClient> Client() const { return m_Client; }
				__forceinline CefMessageRouterBrowserSide::Handler* Handler() const { return m_Handler; }

			protected:
				void RenderUI();
				void OnSwapAngleFrame(void* p_Surface);

				bool m_Internal;
				bool m_Initialized;
				bool m_ShouldRender;

				unsigned long m_Width;
				unsigned long m_Height;

				unsigned short m_Indices[6];

				bool m_HasRenderState;

				LPDIRECT3DTEXTURE9 m_SharedTexture;
				LPDIRECT3DTEXTURE9 m_MainTexture;

				LPDIRECT3DDEVICE9 m_Device;

				HANDLE m_SharedHandle;
				volatile LONG m_SwapCounter;
				bool m_SetSwapHandler;

				FARPROC m_EglGetMainWindowSharedHandle;
				FARPROC m_EglSetSwapFrameHandler;

				CefMessageRouterBrowserSide::Handler* m_Handler;

				CefRefPtr<WebUIClient> m_Client;
			};
		}
	}
}