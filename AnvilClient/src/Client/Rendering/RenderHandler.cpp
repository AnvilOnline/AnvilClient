#include "RenderHandler.hpp"
#include <Utils/Logger.hpp>
#include <internal/cef_ptr.h>
#include <cef_browser.h>
#include <internal/cef_types_wrappers.h>

using Anvil::Client::Rendering::RenderHandler;

RenderHandler::RenderHandler(LPDIRECT3DTEXTURE9 p_Texture) :
	m_Texture(p_Texture)
{

}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect)
{
	if (!m_Texture)
		return false;

	D3DSURFACE_DESC s_SurfaceDesc;
	auto s_Result = m_Texture->GetLevelDesc(0, &s_SurfaceDesc);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not get the surface information of the texture (%x).", s_Result);
		return false;
	}

	p_Rect = CefRect(0, 0, s_SurfaceDesc.Width, s_SurfaceDesc.Height);
	return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height)
{
	if (!m_Texture)
		return;

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
