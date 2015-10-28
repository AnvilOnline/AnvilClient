#include "WebRenderer.hpp"
#include <Utils/Logger.hpp>
#include <cef_app.h>
#include <internal/cef_types_wrappers.h>
#include <internal/cef_win.h>
#include <cef_browser.h>
#include <internal/cef_ptr.h>
#include "BrowserClient.hpp"
#include "RenderHandler.hpp"
#include <windows.h>

using Anvil::Client::Rendering::WebRenderer;

WebRenderer* WebRenderer::m_Instance = nullptr;

WebRenderer::WebRenderer() : 
	m_Sprite(nullptr),
	m_Texture(nullptr),
	m_Width(0),
	m_Height(0)
{
}

unsigned long __stdcall MessageLoop(LPVOID)
{
	CefRunMessageLoop();
	return 0;
}

WebRenderer* WebRenderer::GetInstance()
{
	if (!m_Instance)
		m_Instance = new WebRenderer;
	return m_Instance;
}

bool WebRenderer::Init()
{
	WriteLog("WebRenderer init.");
	return true;
}

bool WebRenderer::InitRenderer(LPDIRECT3DDEVICE9 p_Device)
{
	WriteLog("WebRenderer InitRenderer.");

	if (!p_Device)
		return false;

	// Try to get the current viewport
	D3DVIEWPORT9 s_Viewport = { 0, 0, 0, 0 };
	auto s_Result = p_Device->GetViewport(&s_Viewport);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not get the viewport (%x).", s_Result);
		return false;
	}

	// Update our internal renderer information
	m_Width = s_Viewport.Width;
	m_Height = s_Viewport.Height;

	// Create a sprite for rendering
	s_Result = D3DXCreateSprite(p_Device, &m_Sprite);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not create sprite (%x).", s_Result);
		return false;
	}

	// Create a font for debug purposes
	s_Result = D3DXCreateFont(p_Device, 16, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_Font);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not create font (%x).", s_Result);
		return false;
	}

	// Create a texture
	s_Result = p_Device->CreateTexture(m_Width, m_Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_Texture, nullptr);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not create texture (%x).", s_Result);
		return false;
	}

	CefMainArgs s_MainArgs;

	s_Result = CefExecuteProcess(s_MainArgs, nullptr, nullptr);
	if (s_Result >= 0)
	{
		WriteLog("Child process has ended, so exit.");
		return false;
	}
	if (s_Result == -1)
	{
		// We are int he father process.
		WriteLog("Father process.");
	}

	CefSettings s_Settings;

	s_Result = CefInitialize(s_MainArgs, s_Settings, nullptr, nullptr);
	if (!s_Result)
	{
		WriteLog("Handle error.");
		return false;
	}

	CefRefPtr<CefBrowser> s_Browser;
	CefRefPtr<BrowserClient> s_BrowserClient;
	{
		CefWindowInfo s_WindowInfo;
		CefBrowserSettings s_BrowserSettings;

		auto s_RenderHandler = new RenderHandler(m_Texture);

		s_BrowserClient = new BrowserClient(s_RenderHandler);
		
		auto s_RenderHandlerPtr = s_BrowserClient->GetRenderHandler();
		WriteLog("RenderHandler: %p.", s_RenderHandlerPtr);

		s_Browser = CefBrowserHost::CreateBrowserSync(s_WindowInfo, s_BrowserClient.get(), "http://google.com", s_BrowserSettings, nullptr);

	}
	

	return true;
}

bool WebRenderer::Render(LPDIRECT3DDEVICE9 p_Device)
{
	if (!p_Device)
		return false;

	return true;
}