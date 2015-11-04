#include "WebUIFrame.hpp"
#include "WebUIClient.hpp"
#include <boost/phoenix/function/function.hpp>

using namespace Anvil::Client::Rendering;

WebUIFrame::WebUIFrame(bool p_Internal, unsigned long p_Width, unsigned long p_Height, CefMessageRouterBrowserSide::Handler* p_Handler) :
	m_Internal(p_Internal),
	m_Width(p_Width),
	m_Height(p_Height),
	m_Initialized(false),
	m_SharedTexture(nullptr),
	m_MainTexture(nullptr),
	m_EglGetMainWindowSharedHandle(nullptr),
	m_EglSetSwapFrameHandler(nullptr),
	m_Handler(p_Handler),
	m_SharedHandle(nullptr),
	m_Client(nullptr),
	m_SetSwapHandler(false)
{
	InterlockedExchange(&m_SwapCounter, 0);

	memset(&m_Indices, 0, sizeof(m_Indices));

	// Set the indices.
	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 3;
	m_Indices[3] = 1;
	m_Indices[4] = 2;
	m_Indices[5] = 3;
}

WebUIFrame::~WebUIFrame()
{
	m_Initialized = false;

	if (m_Client.get())
	{
		delete m_Client.get();
		m_Client = nullptr;
	}

	if (m_SharedTexture)
	{
		m_SharedTexture->Release();
		m_SharedTexture = nullptr;
	}

	if (m_MainTexture)
	{
		m_MainTexture->Release();
		m_MainTexture = nullptr;
	}
}

bool WebUIFrame::Init(const std::string& p_URL, LPDIRECT3DDEVICE9 p_Device)
{
	if (m_Initialized)
		return true;

	m_Client = new WebUIClient(this);

	CefWindowInfo s_Info;
	RECT s_Rect = { 0, 0, 1920, 1080 };
	s_Info.SetAsWindowless(NULL, true);

	CefBrowserSettings s_BrowserSettings;

	s_BrowserSettings.windowless_frame_rate = 60;
	s_BrowserSettings.webgl = STATE_ENABLED;
	/*s_BrowserSettings.java = STATE_DISABLED;*/
	s_BrowserSettings.local_storage = STATE_DISABLED;
	s_BrowserSettings.databases = STATE_ENABLED;
	s_BrowserSettings.application_cache = STATE_DISABLED;
	s_BrowserSettings.file_access_from_file_urls = STATE_DISABLED;
	s_BrowserSettings.javascript_close_windows = STATE_DISABLED;
	s_BrowserSettings.javascript_open_windows = STATE_DISABLED;
	s_BrowserSettings.javascript_access_clipboard = STATE_DISABLED;
	s_BrowserSettings.universal_access_from_file_urls = STATE_DISABLED;

	auto s_RequestContext = CefRequestContext::GetGlobalContext();

	if (!CefBrowserHost::CreateBrowser(s_Info, m_Client, p_URL.c_str(), s_BrowserSettings, s_RequestContext))
		return false;

	m_Device = p_Device;

	m_Initialized = true;

	return true;
}

void WebUIFrame::Update()
{
	if (!m_Initialized)
		return;

	if (!m_Device)
		return;

	if (!m_ShouldRender)
		return;

	RenderUI();
}

void WebUIFrame::OnResized(unsigned long p_Width, unsigned long p_Height)
{
	// Set the new dimensions.
	m_Width = p_Width;
	m_Height = p_Height;
}

void WebUIFrame::Render()
{
	if (!m_Initialized)
		return;

	if (!m_Device)
		return;

	if (!m_ShouldRender)
		return;

	if (!m_EglSetSwapFrameHandler || !m_EglGetMainWindowSharedHandle)
	{
		auto s_Library = GetModuleHandleA("libEGL.dll");

		if (s_Library == NULL)
			return;

		m_EglGetMainWindowSharedHandle = GetProcAddress(s_Library, reinterpret_cast<LPCSTR>(100));
		m_EglSetSwapFrameHandler = GetProcAddress(s_Library, reinterpret_cast<LPCSTR>(102));

		if (!m_EglSetSwapFrameHandler || !m_EglGetMainWindowSharedHandle)
			return;
	}

	D3DVIEWPORT9 s_Viewport;
	if (m_Device->GetViewport(&s_Viewport) != D3D_OK)
		return;

	if (s_Viewport.Height != m_Height ||
		s_Viewport.Width != m_Width)
	{
		// Trigger the resize events.
		OnResized(s_Viewport.Width, s_Viewport.Height);
		static_cast<WebUIClient*>(m_Client.get())->Browser()->GetHost()->WasResized();
	}

	HANDLE s_SharedHandle = nullptr;

	if (!reinterpret_cast<unsigned int(*)(HANDLE*)>(m_EglGetMainWindowSharedHandle)(&s_SharedHandle))
		return;

	if (s_SharedHandle == nullptr)
		return;

	// TODO: Figure out rendering code

}

void WebUIFrame::RenderUI()
{
	// TODO: Implement
}

void WebUIFrame::ExecuteJavascript(const std::string& p_Code)
{
	if (m_Client.get())
		m_Client->ExecuteJavascript(p_Code);
}

void WebUIFrame::OnSwapAngleFrame(void* p_Surface)
{
	//printf("Swapping\n");
	InterlockedIncrement(&m_SwapCounter);
}