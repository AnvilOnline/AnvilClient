#pragma once
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/wrapper/cef_message_router.h>
#include <queue>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebUIFrame;

			class WebUIClient :
				public CefClient,
				public CefLifeSpanHandler,
				public CefDisplayHandler,
				public CefContextMenuHandler,
				public CefLoadHandler,
				public CefRenderHandler,
				public CefRequestHandler
			{
			public:
				WebUIClient(WebUIFrame* p_Frame);
				virtual ~WebUIClient();

			protected:
				virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
				virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
				virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
				virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
				virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
				virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }

			protected:
				virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> p_Browser, CefProcessId p_SourceProcess, CefRefPtr<CefProcessMessage> p_Message) override;

				virtual void OnAfterCreated(CefRefPtr<CefBrowser> p_Browser) override;

				virtual void OnLoadStart(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame) override;

				virtual void OnLoadEnd(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, int p_StatusCode) override;

				virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefContextMenuParams> p_Params, CefRefPtr<CefMenuModel> p_Model) override;

				virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> p_Browser, const CefString& p_Message, const CefString& p_Source, int p_Line) override;

				virtual bool GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect) override;

				virtual void OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height) override;

				virtual void OnPopupShow(CefRefPtr<CefBrowser> p_Browser, bool p_Show) override;

				virtual void OnPopupSize(CefRefPtr<CefBrowser> p_Browser, const CefRect& p_Rect) override;

				virtual void OnBeforeClose(CefRefPtr<CefBrowser> p_Browser) override;

				virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> p_Browser, TerminationStatus p_Status) override;

				virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefRequest> p_Request, bool p_Redirect) override;

			public:
				__forceinline CefRefPtr<CefBrowser> Browser() const { return m_Browser; }

				void ExecuteJavascript(const std::string& p_Code);

			protected:
				std::queue<std::string> m_QueuedJavascript;

				CefRefPtr<CefBrowser> m_Browser;
				CefRefPtr<CefMessageRouterBrowserSide> m_BrowserSideRouter;

				WebUIFrame* m_Frame;

				bool m_Loaded;

				IMPLEMENT_REFCOUNTING(WebUIClient);
			};
		}
	}
}
