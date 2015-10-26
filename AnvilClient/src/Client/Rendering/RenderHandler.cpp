#include "RenderHandler.hpp"
#include <Utils/Logger.hpp>
#include <internal/cef_ptr.h>
#include <cef_browser.h>
#include <internal/cef_types_wrappers.h>

using Anvil::Client::Rendering::RenderHandler;

RenderHandler::RenderHandler() :
	m_Texture(nullptr)
{

}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect)
{
	// TODO: Set the rect with 0,0, m_Texture->Width and m_Texture->Height
	return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height)
{
	D3DLOCKED_RECT s_Rect;
	auto s_Ret = m_Texture->LockRect(0, &s_Rect, nullptr, D3DLOCK_DISCARD);
	if (SUCCEEDED(s_Ret))
	{
		memcpy(s_Rect.pBits, p_Buffer, p_Width * p_Height * 4);
		m_Texture->UnlockRect(0);
	}
	else
		WriteLog("Could not lock rect.");
}
