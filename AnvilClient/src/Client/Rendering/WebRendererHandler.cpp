#include "WebRendererHandler.hpp"
#include <Utils/Logger.hpp>

using namespace Anvil::Client::Rendering;

WebRendererHandler::WebRendererHandler(LPDIRECT3DDEVICE9 p_Device) :
	m_Device(p_Device),
	m_TextureData(nullptr)
{
}

bool WebRendererHandler::GetViewRect(CefRefPtr<CefBrowser> p_Browser, CefRect& p_Rect)
{
	unsigned long s_Width = 0, s_Height = 0;
	if (!GetViewportInformation(s_Width, s_Height))
		return false;

	p_Rect = CefRect(0, 0, s_Width, s_Height);
	return true;
}

void WebRendererHandler::OnPaint(CefRefPtr<CefBrowser> p_Browser, PaintElementType p_Type, const RectList& p_DirtyRects, const void* p_Buffer, int p_Width, int p_Height)
{
	if (!m_TextureData)
		return;

	m_TextureLock.lock();

	for (auto& l_Rect : p_DirtyRects)
	{
		memcpy(m_TextureData, p_Buffer, p_Width * p_Height * 4);

		//const unsigned int* l_Source = nullptr;
		//unsigned int* l_Dest = nullptr;
		//__m128i brMask = _mm_set1_epi32(0x00ff00ff);

		//for (auto y = 0; y < l_Rect.height; y++)
		//{
		//	l_Source = reinterpret_cast<const unsigned int*>(static_cast<const unsigned char*>(p_Buffer) + (y + l_Rect.y) * (p_Width * 4) + l_Rect.x * 4);
		//	l_Dest = reinterpret_cast<unsigned int*>(m_TextureData) + (y + l_Rect.y) * (p_Width * 4) + l_Rect.x * 4;
		//	auto x = 0;

		//	// Make output writes aligned
		//	for (x = 0; ((reinterpret_cast<intptr_t>(&l_Dest[x]) & 15) != 0) && x < l_Rect.width; x++)
		//	{
		//		auto rgba = l_Source[x];
		//		l_Dest[x] = (_rotl(rgba, 16) & 0x00ff00ff) | (rgba & 0xff00ff00);
		//	}

		//	for (; x + 3 < l_Rect.width; x += 4)
		//	{
		//		__m128i sourceData = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&l_Source[x]));
		//		// Mask out g and a, which don't change
		//		__m128i gaComponents = _mm_andnot_si128(brMask, sourceData);
		//		// Mask out b and r
		//		__m128i brComponents = _mm_and_si128(sourceData, brMask);
		//		// Swap b and r
		//		__m128i brSwapped = _mm_shufflehi_epi16(_mm_shufflelo_epi16(brComponents, _MM_SHUFFLE(2, 3, 0, 1)), _MM_SHUFFLE(2, 3, 0, 1));
		//		__m128i result = _mm_or_si128(gaComponents, brSwapped);
		//		_mm_store_si128(reinterpret_cast<__m128i*>(&l_Dest[x]), result);
		//	}

		//	// Perform leftover writes
		//	for (; x < l_Rect.width; x++)
		//	{
		//		auto rgba = l_Source[x];
		//		l_Dest[x] = (_rotl(rgba, 16) & 0x00ff00ff) | (rgba & 0xff00ff00);
		//	}
		//}
	}

	m_TextureLock.unlock();
}

bool WebRendererHandler::GetViewportInformation(unsigned long& p_Width, unsigned long& p_Height)
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

bool WebRendererHandler::Resize(unsigned long p_Width, unsigned long p_Height)
{
	WriteLog("Resize called: %dx%d.", p_Width, p_Height);
	auto s_TextureDataSize = p_Width * p_Height * 4;
	if (!s_TextureDataSize)
		return false;

	m_TextureLock.lock();

	WriteLog("Tried to allocate 0x%x bytes.", s_TextureDataSize);
	m_TextureData = static_cast<unsigned char*>(malloc(s_TextureDataSize));

	WriteLog("1");
	if (!m_TextureData)
		return false;

	WriteLog("2");
	memset(m_TextureData, 0, s_TextureDataSize);

	WriteLog("3");
	m_TextureLock.unlock();

	WriteLog("4");
	return true;
}

unsigned char* WebRendererHandler::GetTexture()
{
	return m_TextureData;
}