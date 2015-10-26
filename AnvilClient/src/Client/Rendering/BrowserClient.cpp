#include "BrowserClient.hpp"
#include "RenderHandler.hpp"

using namespace Anvil::Client::Rendering;

BrowserClient::BrowserClient(RenderHandler* p_RenderHandler) : m_RenderHandler(p_RenderHandler)
{
}

BrowserClient::~BrowserClient()
{
}

CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler()
{
	return m_RenderHandler;
}