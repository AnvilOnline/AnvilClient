#pragma once
#include <cef_render_handler.h>
#include <cef_client.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebRendererHandler;

			class WebRendererClient : public CefClient
			{
			protected:
				CefRefPtr<CefRenderHandler> m_RenderHandler;

			public:
				WebRendererClient(CefRenderHandler* p_RenderHandler);

				virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

			protected:
				IMPLEMENT_REFCOUNTING(WebRendererClient);
			};
		}
	}
}