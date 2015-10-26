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

bool WebRenderer::Init()
{
	// TODO: Properly initialize everything
	auto s_Result = D3DXCreateSprite(nullptr, &m_Sprite);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not create sprite.");
		return false;
	}

	s_Result = D3DXCreateTexture(nullptr, m_Width, m_Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_Texture);
	if (s_Result != D3D_OK)
	{
		WriteLog("Could not create texture.");
		return false;
	}


	s_Result = CefExecuteProcess(CefMainArgs(), nullptr, nullptr);
	if (s_Result >= 0)
	{
		WriteLog("Child process has ended, so exit.");
		return false;
	}
	if (s_Result == -1)
	{
		// TODO: Something
	}

	CefMainArgs s_Args;
	CefSettings s_Settings;

	s_Result = CefInitialize(s_Args, s_Settings, nullptr, nullptr);

	CreateThread(nullptr, 0, MessageLoop, nullptr, 0, nullptr);

	CefRefPtr<CefBrowser> s_Browser;
	CefRefPtr<BrowserClient> s_BrowserClient;
	{
		CefWindowInfo s_WindowInfo;
		CefBrowserSettings s_BrowserSettings;

		auto s_RenderHandler = new RenderHandler();
		
		s_BrowserClient = new BrowserClient(s_RenderHandler);
	}
	return true;
}
