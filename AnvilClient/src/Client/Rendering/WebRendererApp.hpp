#pragma once
#include <include/cef_app.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebRendererApp : 
				public CefApp,
				public CefBrowserProcessHandler,
				public CefRenderProcessHandler
			{
			public:
				virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> p_Registrar) override;

				virtual CefRefPtr< CefBrowserProcessHandler > GetBrowserProcessHandler() override;
				virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
				virtual void OnContextInitialized() override;
				virtual void OnContextCreated(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context) override;

			protected:
				IMPLEMENT_REFCOUNTING(WebRendererApp);
			};
		}
	}
}