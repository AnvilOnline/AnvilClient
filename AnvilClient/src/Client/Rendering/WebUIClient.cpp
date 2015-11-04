#include "WebUIClient.hpp"
#include "WebUIFrame.hpp"

using namespace Anvil::Client::Rendering;

WebUIClient::WebUIClient(WebUIFrame* p_Frame)
	: m_Frame(p_Frame), m_Loaded(false)
{

}

WebUIClient::~WebUIClient()
{

}

bool WebUIClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> p_Browser, CefProcessId p_SourceProcess, CefRefPtr<CefProcessMessage> p_Message)
{
	return m_BrowserSideRouter->OnProcessMessageReceived(p_Browser, p_SourceProcess, p_Message);
}

void WebUIClient::OnAfterCreated(CefRefPtr<CefBrowser> p_Browser)
{
	m_Browser = p_Browser;

	CefMessageRouterConfig s_Config;
	s_Config.js_query_function = "aoQuery";
	s_Config.js_cancel_function = "aoQueryCancel";
	m_BrowserSideRouter = CefMessageRouterBrowserSide::Create(s_Config);

	m_BrowserSideRouter->AddHandler(m_Frame->Handler(), true);

	m_Browser->GetHost()->SetFocus(true);
	m_Browser->GetHost()->SendFocusEvent(true);
}

void WebUIClient::OnLoadStart(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame)
{
}

void WebUIClient::OnLoadEnd(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, int p_StatusCode)
{
	m_Loaded = true;

	m_Browser->GetHost()->SetFocus(true);
	m_Browser->GetHost()->SendFocusEvent(true);

	while (!m_QueuedJavascript.empty())
	{
		m_Browser->GetMainFrame()->ExecuteJavaScript(m_QueuedJavascript.front(), "internal", 0);
		m_QueuedJavascript.pop();
	}
}

void WebUIClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefContextMenuParams> p_Params, CefRefPtr<CefMenuModel> p_Model)
{

}

bool WebUIClient::OnConsoleMessage(CefRefPtr<CefBrowser> p_Browser, const CefString& p_Message, const CefString& p_Source, int p_Line)
{
	return false;
}

bool WebUIClient::GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect)
{
	p_Rect.Set(0, 0, m_Frame->FrameWidth(), m_Frame->FrameHeight());
	return true;
}

void WebUIClient::OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height)
{
}

void WebUIClient::OnPopupShow(CefRefPtr<CefBrowser> p_Browser, bool p_Show)
{

}

void WebUIClient::OnPopupSize(CefRefPtr<CefBrowser> p_Browser, const CefRect& p_Rect)
{

}

void WebUIClient::ExecuteJavascript(const std::string& p_Code)
{
	if (m_Loaded)
	{
		m_Browser->GetMainFrame()->ExecuteJavaScript(p_Code, "internal", 0);
		return;
	}

	m_QueuedJavascript.push(p_Code);
}

void WebUIClient::OnBeforeClose(CefRefPtr<CefBrowser> p_Browser)
{
	m_BrowserSideRouter->OnBeforeClose(p_Browser);
}

void WebUIClient::OnRenderProcessTerminated(CefRefPtr<CefBrowser> p_Browser, TerminationStatus p_Status)
{
	m_BrowserSideRouter->OnRenderProcessTerminated(p_Browser);
}

bool WebUIClient::OnBeforeBrowse(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefRequest> p_Request, bool p_Redirect)
{
	m_BrowserSideRouter->OnBeforeBrowse(p_Browser, p_Frame);
	return false;
}