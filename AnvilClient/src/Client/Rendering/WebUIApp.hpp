#pragma once
#include <include/cef_app.h>
#include <include/wrapper/cef_message_router.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebUIApp :
				public CefApp,
				public CefRenderProcessHandler,
				public CefV8Handler
			{
			public:
				WebUIApp();
				virtual ~WebUIApp();

			public:
				virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

			public:
				virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> p_Registrar) override;

				virtual void OnContextCreated(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefV8Context> p_Context) override;

				virtual void OnContextReleased(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefV8Context> p_Context) override;

				virtual void OnWebKitInitialized() override;

			protected:
				virtual void OnBeforeCommandLineProcessing(const CefString& p_ProcessType, CefRefPtr<CefCommandLine> p_CommandLine) override;

				virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> p_Browser, CefProcessId p_SourceProcess, CefRefPtr<CefProcessMessage> p_Message) override;

				virtual bool Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception) override;

			protected:
				CefRefPtr<CefMessageRouterRendererSide> m_RendererSideRouter;

			protected:
				IMPLEMENT_REFCOUNTING(WebUIApp);
			};
		}
	}
}