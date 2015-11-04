#include "WebUIApp.hpp"
#include <include/cef_process_message.h>

using namespace Anvil::Client::Rendering;

WebUIApp::WebUIApp()
{
}

WebUIApp::~WebUIApp()
{
}

void WebUIApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> p_Registrar)
{
	p_Registrar->AddCustomScheme("ao", true, false, false);
}

void WebUIApp::OnContextCreated(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefV8Context> p_Context)
{
	m_RendererSideRouter->OnContextCreated(p_Browser, p_Frame, p_Context);
}

void WebUIApp::OnContextReleased(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, CefRefPtr<CefV8Context> p_Context)
{
	m_RendererSideRouter->OnContextReleased(p_Browser, p_Frame, p_Context);
}

void WebUIApp::OnWebKitInitialized()
{
	CefMessageRouterConfig s_Config;
	s_Config.js_query_function = "aoQuery";
	s_Config.js_cancel_function = "aoQueryCancel";
	m_RendererSideRouter = CefMessageRouterRendererSide::Create(s_Config);
}

void WebUIApp::OnBeforeCommandLineProcessing(const CefString& p_ProcessType, CefRefPtr<CefCommandLine> p_CommandLine)
{
	p_CommandLine->AppendSwitchWithValue("off-screen-frame-rate", "60");
	p_CommandLine->AppendSwitch("enable-experimental-web-platform-features");
	p_CommandLine->AppendSwitch("in-process-gpu");
}

bool WebUIApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> p_Browser, CefProcessId p_SourceProcess, CefRefPtr<CefProcessMessage> p_Message)
{
	return m_RendererSideRouter->OnProcessMessageReceived(p_Browser, p_SourceProcess, p_Message);
}

bool WebUIApp::Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception)
{
	return false;
}