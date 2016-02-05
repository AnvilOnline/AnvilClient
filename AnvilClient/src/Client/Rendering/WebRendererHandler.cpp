/*
Code was used from NoFaTe (http://nofate.me)
*/
#include "WebRendererHandler.hpp"
#include <Utils/Logger.hpp>

using namespace Anvil::Client::Rendering;

WebRendererHandler::WebRendererHandler(LPDIRECT3DDEVICE9 p_Device) :
	m_Device(p_Device),
	m_TextureData(nullptr),
	m_Browser(nullptr)
{
}

bool WebRendererHandler::GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect)
{
	uint32_t s_Width = 0, s_Height = 0;
	if (!GetViewportInformation(s_Width, s_Height))
		return false;

	p_Rect = CefRect(0, 0, s_Width, s_Height);
	return true;
}

void WebRendererHandler::OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height)
{
	if (!m_Browser)
		m_Browser = p_Browser;

	if (!m_TextureData)
		return;

	m_TextureLock.lock();

	for (auto& l_Rect : p_DirtyRects)
		memcpy(m_TextureData, p_Buffer, p_Width * p_Height * 4);

	m_TextureLock.unlock();
}

bool WebRendererHandler::GetViewportInformation(uint32_t& p_Width, uint32_t& p_Height)
{
	if (!m_Device)
		return false;

	D3DVIEWPORT9 s_Viewport;
	auto s_Ret = m_Device->GetViewport(&s_Viewport);
	if (FAILED(s_Ret))
		return false;

	p_Width = s_Viewport.Width;
	p_Height = s_Viewport.Height;

	return true;
}

bool WebRendererHandler::Resize(uint32_t p_Width, uint32_t p_Height)
{
	WriteLog("Resize called: %dx%d.", p_Width, p_Height);
	auto s_TextureDataSize = p_Width * p_Height * 4;
	if (!s_TextureDataSize)
		return false;

	m_TextureLock.lock();

	WriteLog("Tried to allocate 0x%x bytes.", s_TextureDataSize);
	// If we get a resize, don't leak all of that memory it's bad
	if (m_TextureData)
	{
		delete m_TextureData;
		m_TextureData = nullptr;
	}

	m_TextureData = new uint8_t[s_TextureDataSize];
	if (!m_TextureData)
		return false;

	memset(m_TextureData, 0, s_TextureDataSize);
	m_TextureLock.unlock();
	return true;
}

uint8_t* WebRendererHandler::GetTexture()
{
	return m_TextureData;
}

CefRefPtr<CefBrowser> WebRendererHandler::GetBrowser()
{
	return m_Browser;
}

void WebRendererHandler::LockTexture()
{
	m_TextureLock.lock();
}

void WebRendererHandler::UnlockTexture()
{
	m_TextureLock.unlock();
}