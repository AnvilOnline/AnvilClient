#pragma once
#include <internal/cef_ptr.h>
#include <internal/cef_types_wrappers.h>
#include <cef_browser.h>
#include <cef_render_handler.h>
#include <d3d9.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class RenderHandler : public CefRenderHandler
			{
			protected:
				~RenderHandler()
				{
				}

			private:
				LPDIRECT3DTEXTURE9 m_Texture;

			public:
				RenderHandler();

				bool GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect) override;
				void OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height) override;

				IMPLEMENT_REFCOUNTING(RenderHandler);
			};
		}
	}
}