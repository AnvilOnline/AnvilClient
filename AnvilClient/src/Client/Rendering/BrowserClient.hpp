#pragma once
#include <cef_client.h>
#include "RenderHandler.hpp"
#include <internal/cef_ptr.h>
#include <cef_render_handler.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class RenderHandler;

			class BrowserClient : public CefClient
			{
			protected:
				~BrowserClient();

			private:
				CefRefPtr<CefRenderHandler> m_RenderHandler;
			public:
				BrowserClient(RenderHandler* p_RenderHandler);

				virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

				IMPLEMENT_REFCOUNTING(BrowserClient);
			};
		}
	}
}