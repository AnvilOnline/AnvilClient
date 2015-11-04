#include "WebUISchemeHandlerFactory.hpp"
#include "WebUISchemeHandler.hpp"
#include "WebUI.hpp"

using namespace Anvil::Client::Rendering;

CefRefPtr<CefResourceHandler> WebUISchemeHandlerFactory::Create(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, const CefString& p_SchemeName, CefRefPtr<CefRequest> p_Request)
{
	return new WebUISchemeHandler(p_Frame->IsMain(), p_Frame);
}