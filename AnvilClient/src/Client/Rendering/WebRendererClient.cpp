/*
Code was used from NoFaTe (http://nofate.me)
*/

#include "WebRendererClient.hpp"

using namespace Anvil::Client::Rendering;

WebRendererClient::WebRendererClient(CefRenderHandler* p_RenderHandler) :
	m_RenderHandler(p_RenderHandler)
{

}

CefRefPtr<CefRenderHandler> WebRendererClient::GetRenderHandler()
{
	return m_RenderHandler;
}
