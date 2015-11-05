#pragma once
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
				unsigned char* m_TextureData;
				std::mutex m_TextureLock;

			public:
				WebRendererHandler(LPDIRECT3DDEVICE9 p_Device);

				bool GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect &p_Rect) override;
				void OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList &p_DirtyRects, const void *p_Buffer, int p_Width, int p_Height) override;

				bool GetViewportInformation(unsigned long& p_Width, unsigned long& p_Height);
				bool Resize(unsigned long p_Width, unsigned long p_Height);

				unsigned char* GetTexture();

			protected:
				IMPLEMENT_REFCOUNTING(WebRendererHandler);
			};
		}
	}
}