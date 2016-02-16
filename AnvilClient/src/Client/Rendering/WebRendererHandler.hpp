#pragma once
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <d3d9.h>
#include <mutex>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebRendererHandler : public CefRenderHandler
			{
				LPDIRECT3DDEVICE9 m_Device;
				std::vector<uint8_t> m_TextureData;
				std::mutex m_TextureLock;

				CefRefPtr<CefBrowser> m_Browser;

			public:
				WebRendererHandler(LPDIRECT3DDEVICE9 p_Device);

				bool GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect &p_Rect) override;
				void OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList &p_DirtyRects, const void *p_Buffer, int p_Width, int p_Height) override;

				bool GetViewportInformation(uint32_t& p_Width, uint32_t& p_Height);
				bool Resize(uint32_t p_Width, uint32_t p_Height);

				uint8_t* GetTexture();
				uint32_t GetTextureLength();
				CefRefPtr<CefBrowser> GetBrowser();
				void LockTexture();
				void UnlockTexture();

			protected:
				IMPLEMENT_REFCOUNTING(WebRendererHandler);
			};
		}
	}
}