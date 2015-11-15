/*
Code was used from NoFaTe (http://nofate.me)
*/
#include "WebRendererApp.hpp"
#include "Bridge/Client/GetVersion.hpp"
#include <Utils/Logger.hpp>
#include "Bridge/Client/ClientFunctions.hpp"

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
	auto s_BridgeObject = CefV8Value::CreateObject(nullptr);

	// Retrieve the context's window object.
	auto s_GlobalObject = context->GetGlobal();

	// Create an instance of my CefV8Handler object.
	CefRefPtr<CefV8Handler> s_GetVersionHandler = new GetVersionHandler();
	CefRefPtr<CefV8Handler> s_ClientFunctionsHandler = new ClientFunctionsHandler();

	// Create the "myfunc" function.
	auto s_GetVersionFunc = CefV8Value::CreateFunction("GetVersion", s_GetVersionHandler);
	auto s_ClientQuit = CefV8Value::CreateFunction("Quit", s_ClientFunctionsHandler);
	auto s_ClientConnect = CefV8Value::CreateFunction("Connect", s_ClientFunctionsHandler);

	// Add the "GetVersion" function to the "BridgeObject" object.
	s_BridgeObject->SetValue("GetVersion", s_GetVersionFunc, V8_PROPERTY_ATTRIBUTE_NONE);
	s_BridgeObject->SetValue("Quit", s_ClientQuit, V8_PROPERTY_ATTRIBUTE_NONE);
	s_BridgeObject->SetValue("Connect", s_ClientConnect, V8_PROPERTY_ATTRIBUTE_NONE);

	s_GlobalObject->SetValue("anvil", s_BridgeObject, V8_PROPERTY_ATTRIBUTE_NONE);

	WriteLog("OnContextCreated.");
}