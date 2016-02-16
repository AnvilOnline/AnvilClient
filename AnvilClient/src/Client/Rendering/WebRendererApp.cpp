/*
Code was used from NoFaTe (http://nofate.me)
*/
#include "WebRendererApp.hpp"
#include <Utils/Logger.hpp>
#include "Bridge/Client/ClientFunctions.hpp"
#include "Bridge/WebRendererBridge.hpp"

using namespace Anvil::Client::Rendering;

void WebRendererApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> p_Registrar)
{
	p_Registrar->AddCustomScheme("anvil", true, false, false);
}

CefRefPtr<CefBrowserProcessHandler> WebRendererApp::GetBrowserProcessHandler()
{
	//WriteLog("GetBrowserProcessHandler.");
	return this;
}

CefRefPtr<CefRenderProcessHandler> WebRendererApp::GetRenderProcessHandler()
{
	//WriteLog("GetRenderProcessHandler.");
	return this;
}

void WebRendererApp::OnContextInitialized()
{
	//WriteLog("OnContextInitialized.");
}

void WebRendererApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	WriteLog("OnContextCreated.");

	m_Bridge = std::make_shared<Bridge::WebRendererBridge>();
	if (!m_Bridge)
		return;

	auto s_AnvilObject = m_Bridge->CreateHandler("anvil");
	if (!s_AnvilObject)
		return;

	s_AnvilObject->AddFunction("GetVersion", Bridge::ClientFunctions::OnGetVersion);
	s_AnvilObject->AddFunction("Connect", Bridge::ClientFunctions::OnConnect);
	s_AnvilObject->AddFunction("LoadMap", Bridge::ClientFunctions::OnLoadMap);
	s_AnvilObject->AddFunction("Quit", Bridge::ClientFunctions::OnQuit);

	m_Bridge->AddHandler(s_AnvilObject);
}