#include "WebRenderer.hpp"
#include "WebRendererHandler.hpp"
#include "WebRendererSchemeHandlerFactory.hpp"
#include <cef_app.h>
#include <cef_origin_whitelist.h>

#include <d3dx9.h>
#include <Utils/Logger.hpp>
#include "WebRendererApp.hpp"
#include <boost/format/free_funcs.hpp>

using Anvil::Client::Rendering::WebRenderer;

WebRenderer* WebRenderer::m_Instance = nullptr;
WebRenderer::WebRenderer() : 
	m_Client(nullptr),
	m_RenderHandler(nullptr),
	m_App(nullptr),
	m_SchemeHandlerFactory(nullptr),
	m_State(RendererState_Disabled),
	m_Texture(nullptr),
	m_Device(nullptr),
	m_Sprite(nullptr),
	m_Font(nullptr)
{
	WriteLog("WebRenderer Ctor.");
}

std::string WebRenderer::GetUIDirectory()
{
	char s_PathBuffer[MAX_PATH] = { 0 };
	auto s_BufferSize = sizeof(s_PathBuffer);

	auto s_Result = GetModuleFileName(nullptr, s_PathBuffer, s_BufferSize);
	if (!s_Result)
	{
		WriteLog("Could not get current executable file path (%x).", GetLastError());
	}

	auto s_LocalExecutable = std::string(s_PathBuffer);
	auto s_RunningDirectory = s_LocalExecutable.substr(0, s_LocalExecutable.find_last_of("\\/"));

	return s_RunningDirectory + "/ui";
}

WebRenderer* WebRenderer::GetInstance()
{
	if (!m_Instance)
		m_Instance = new WebRenderer;
	return m_Instance;
}

bool WebRenderer::Init()
{
	// Do not try to initialize unless we are already at a disabled state
	if (GetState() != RendererState_Startup)
		return false;

	if (!m_Device)
		return false;

	if (m_App.get())
		delete m_App.get();

	if (m_SchemeHandlerFactory)
		delete m_SchemeHandlerFactory;

	m_SchemeHandlerFactory = new WebRendererSchemeHandlerFactory();

	m_App = new WebRendererApp();

	if (!m_App || !m_SchemeHandlerFactory)
	{
		WriteLog("App (%p) or Schema handler (%p) is invalid.", m_App, m_SchemeHandlerFactory);
		Shutdown();
		return false;
	}

	CefMainArgs s_Args(GetModuleHandle(nullptr));

	auto s_Result = CefExecuteProcess(s_Args, m_App, nullptr);
	if (s_Result >= 0)
	{
		WriteLog("CefExecuteProcess failed (%x).", s_Result);
		TerminateProcess(GetCurrentProcess(), 0); // TODO: Take less drastic measures...
		return false;
	}

	CefSettings s_Settings;
	s_Settings.multi_threaded_message_loop = true;
	CefString(&s_Settings.product_version) = "AnvilOnline";
	CefString(&s_Settings.browser_subprocess_path) = "cefsimple.exe"; // TODO: see about pre-compiling this
	s_Settings.no_sandbox = true;
	s_Settings.pack_loading_disabled = false;
	s_Settings.windowless_rendering_enabled = true;
	s_Settings.ignore_certificate_errors = true;
	s_Settings.log_severity = LOGSEVERITY_VERBOSE;
	s_Settings.single_process = true;
#if _DEBUG
	s_Settings.remote_debugging_port = 8884;
#endif

	if (!CefInitialize(s_Args, s_Settings, m_App, nullptr))
	{
		WriteLog("CefInitialize failed.");
		ExitProcess(0); // TODO: Take less drastic measures
		return false;
	}

	// Register our custom handlers
	CefRegisterSchemeHandlerFactory("anvil", "", m_SchemeHandlerFactory);
	CefAddCrossOriginWhitelistEntry("anvil://menu", "http", "", true);

	WriteLog("Handlers registered.");

	auto s_UIDirectory = GetUIDirectory();

	WriteLog("UI Directory: %s", s_UIDirectory.c_str());

	m_RenderHandler = new WebRendererHandler(m_Device);
	if (!m_RenderHandler)
	{
		WriteLog("RenderHandler failed to create.");

		// This should clear out and free all allocated resources up to this point.
		Shutdown();
		return false;
	}

	CefWindowInfo s_WindowInfo;
	CefBrowserSettings s_BrowserSettings;

	s_BrowserSettings.windowless_frame_rate = 60;
	s_BrowserSettings.webgl = STATE_ENABLED;
	s_BrowserSettings.java = STATE_DISABLED;
	s_BrowserSettings.local_storage = STATE_DISABLED;
	s_BrowserSettings.databases = STATE_ENABLED;
	s_BrowserSettings.application_cache = STATE_DISABLED;
	s_BrowserSettings.file_access_from_file_urls = STATE_DISABLED;
	s_BrowserSettings.javascript_close_windows = STATE_DISABLED;
	s_BrowserSettings.javascript_open_windows = STATE_DISABLED;
	s_BrowserSettings.javascript_access_clipboard = STATE_DISABLED;
	s_BrowserSettings.universal_access_from_file_urls = STATE_DISABLED;

	D3DDEVICE_CREATION_PARAMETERS s_Parameters;
	ZeroMemory(&s_Parameters, sizeof(s_Parameters));

	s_Result = m_Device->GetCreationParameters(&s_Parameters);
	if (FAILED(s_Result))
	{
		WriteLog("Could not get the creation parameters.");
		Shutdown();
		return false;
	}

	
	// Jquery fails here, TODO: Implement custom handler
	std::string s_ContainerPath = "anvil://menu/index.html"; /*+ s_UIDirectory + "/container.html";*/

	WriteLog("Container Path: %s.", s_ContainerPath.c_str());

	s_WindowInfo.SetAsWindowless(s_Parameters.hFocusWindow, true);

	m_Client = new WebRendererClient(m_RenderHandler);
	if (!m_Client)
	{
		WriteLog("WebRendererClient failed to initialize.");
		Shutdown();
		return false;
	}

	auto s_RequestContext = CefRequestContext::GetGlobalContext();
	if (!CefBrowserHost::CreateBrowser(s_WindowInfo, m_Client.get(), s_ContainerPath.c_str(), s_BrowserSettings, s_RequestContext))
	{
		WriteLog("Failed to initialize WebRenderer.");
		Shutdown();
		return false;
	}

	// Fucking wizardry hacks
	auto s_RenderHandler = static_cast<WebRendererHandler*>(m_RenderHandler.get());
	if (!s_RenderHandler)
	{
		Shutdown();
		return false;
	}

	uint32_t s_Width = 0, s_Height = 0;
	if (!static_cast<WebRendererHandler*>(m_RenderHandler.get())->GetViewportInformation(s_Width, s_Height))
	{
		Shutdown();
		return false;
	}
		

	if (!Resize(s_Width, s_Height))
	{
		WriteLog("Resize failed.");
		Shutdown();
		return false;
	}

	SetState(RendererState_Hidden);

	return true;
}

bool WebRenderer::InitRenderer(LPDIRECT3DDEVICE9 p_Device)
{
	// In order to enter the state, we have to have already hit the startup state, which init leaves hanging for the dx thread to catch up and call this
	// ^ Thats backwards, initrenderer should be called first
	if (GetState() != RendererState_Disabled)
		return false;

	if (!p_Device)
	{
		WriteLog("Device is invalid.");
		Shutdown();
		return false;
	}

	m_Device = p_Device;

	// Create a sprite for rendering
	auto s_Result = D3DXCreateSprite(m_Device, &m_Sprite);
	if (FAILED(s_Result))
	{
		WriteLog("Could not create sprite (%x).", s_Result);
		Shutdown();
		return false;
	}

	// Create a font for debug purposes
	s_Result = D3DXCreateFont(m_Device, 16, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_Font);
	if (FAILED(s_Result))
	{
		WriteLog("Could not create font (%x).", s_Result);
		Shutdown();
		return false;
	}

	D3DVIEWPORT9 s_Viewport;
	s_Result = p_Device->GetViewport(&s_Viewport);
	if (FAILED(s_Result))
	{
		WriteLog("Could not get viewport (%x).", s_Result);
		Shutdown();
		return false;
	}

	// Create a texture
	s_Result = p_Device->CreateTexture(s_Viewport.Width, s_Viewport.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_Texture, nullptr);
	if (FAILED(s_Result))
	{
		WriteLog("Could not create texture (%x).", s_Result);
		Shutdown();
		return false;
	}

	WriteLog("WebRenderer InitRenderer.");

	// We leave the renderer state in startup so the init function would work
	SetState(RendererState_Startup);

	return true;
}

bool WebRenderer::Render(LPDIRECT3DDEVICE9 p_Device)
{
	auto s_State = GetState();
	if (s_State != RendererState_Overlay && s_State != RendererState_Shown)
		return false;

	if (!p_Device)
	{
		WriteLog("Device is invalid.");
		return false;
	}

	if (!m_Sprite || !m_Font || !m_Texture || !m_RenderHandler)
	{
		WriteLog("Sprite, Font, Texture, Renderhandler invalid.");
		return false;
	}

	auto s_RenderHandler = static_cast<WebRendererHandler*>(m_RenderHandler.get());

	D3DLOCKED_RECT s_Rect;
	auto s_Result = m_Texture->LockRect(0, &s_Rect, nullptr, D3DLOCK_DISCARD);
	if (SUCCEEDED(s_Result))
	{
		uint32_t s_Width = 0, s_Height = 0;
		if (!s_RenderHandler->GetViewportInformation(s_Width, s_Height))
		{
			m_Texture->UnlockRect(0);
			return false;
		}

		auto s_TextureData = s_RenderHandler->GetTexture();
		if (!s_TextureData)
		{
			m_Texture->UnlockRect(0);
			return false;
		}

		memcpy(s_Rect.pBits, s_TextureData, s_Width * s_Height * 4);

		m_Texture->UnlockRect(0);
	}

	if (GetState() == RendererState_Shown)
		p_Device->Clear(1, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 128, 128, 128), 0, 0);

	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_Sprite->Draw(m_Texture, nullptr, nullptr, &D3DXVECTOR3(0, 0, 0), 0xFFFFFFFF);

	m_Sprite->Flush();

	m_Sprite->End();

	return true;
}

bool WebRenderer::Update()
{
	auto s_State = GetState();
	if (s_State != RendererState_Overlay || s_State != RendererState_Shown)
		return false;

	return true;
}

bool WebRenderer::Initialized()
{
	auto s_State = GetState();
	return (s_State == RendererState_Overlay
		|| s_State == RendererState_Shown
		|| s_State == RendererState_Hidden);
}

bool WebRenderer::IsRendering()
{
	return GetState() == RendererState_Overlay || GetState() == RendererState_Shown;
}

bool WebRenderer::ShowRenderer(bool p_Show, bool p_Overlay)
{
	auto s_State = GetState();
	if (s_State == RendererState_Hidden && p_Show)
	{
		SetState(p_Overlay ? RendererState_Overlay : RendererState_Shown);
		return true;
	}

	if ((s_State == RendererState_Overlay || s_State == RendererState_Shown) && !p_Show)
	{
		SetState(RendererState_Hidden);
		return true;
	}

	return false;
}

bool WebRenderer::Resize(uint32_t p_Width, uint32_t p_Height)
{
	if (!m_RenderHandler)
		return false;

	auto s_Result = 
		static_cast<WebRendererHandler*>(m_RenderHandler.get())->Resize(p_Width, p_Height);

	if (!s_Result)
	{
		WriteLog("Resize failed.");
		return false;
	}

	return true;
}

bool WebRenderer::UpdateMouse(uint32_t p_X, uint32_t p_Y)
{
	if (!m_RenderHandler || GetState() != RendererState_Shown)
		return false;

	auto s_Browser = reinterpret_cast<WebRendererHandler*>(m_RenderHandler.get())->GetBrowser().get();
	if (!s_Browser)
		return false;

	CefMouseEvent s_Event;
	s_Event.x = p_X;
	s_Event.y = p_Y;
	
	s_Browser->GetHost()->SendMouseMoveEvent(s_Event, false);

	return true;
}

bool WebRenderer::Click(uint32_t p_X, uint32_t p_Y)
{
	if (!m_RenderHandler || GetState() != RendererState_Shown)
		return false;

	auto s_RenderHandler = reinterpret_cast<WebRendererHandler*>(m_RenderHandler.get());
	if (!s_RenderHandler)
		return false;

	auto s_Browser = s_RenderHandler->GetBrowser().get();
	if (!s_Browser)
		return false;

	CefMouseEvent s_Event;
	s_Event.x = p_X;
	s_Event.y = p_Y;
	s_Event.modifiers = 0;

	auto s_LastClickCount = 1;

	s_Browser->GetHost()->SendMouseClickEvent(s_Event, MBT_LEFT, false, s_LastClickCount);
	s_Browser->GetHost()->SendMouseClickEvent(s_Event, MBT_LEFT, true, s_LastClickCount);
	return true;
}

bool WebRenderer::ExecuteJavascript(std::string p_Code)
{
	auto s_RenderHandler = reinterpret_cast<WebRendererHandler*>(m_RenderHandler.get());
	if (!s_RenderHandler)
		return false;

	auto s_Browser = s_RenderHandler->GetBrowser().get();
	if (!s_Browser)
		return false;

	s_Browser->GetMainFrame()->ExecuteJavaScript(p_Code, "internal", 0);

	return true;
}

bool WebRenderer::PreReset()
{
	if (!m_Device)
		return false;

	if (m_Sprite)
		m_Sprite->OnLostDevice();

	if (m_Font)
		m_Font->OnLostDevice();

	return true;
}

bool WebRenderer::PostReset()
{
	if (!m_Device)
		return false;

	if (m_Sprite)
		m_Sprite->OnResetDevice();

	if (m_Font)
		m_Font->OnResetDevice();

	return true;
}

bool WebRenderer::Shutdown()
{
	// If we are already disabled, don't try to free the elements, and if we are already shutting down don't call it again
	if (m_State == RendererState_Disabled || m_State == RendererState_Shutdown)
		return false;

	// Set us to be in the "shutdown" state and free resources
	SetState(RendererState_Shutdown);

	if (m_RenderHandler)
		delete m_RenderHandler;

	if (m_App)
		delete m_App.get();

	if (m_SchemeHandlerFactory)
		delete m_SchemeHandlerFactory;

	m_RenderHandler = nullptr;
	m_App = nullptr;
	m_SchemeHandlerFactory = nullptr;

	CefShutdown();

	// Set us back to our disabled state
	SetState(RendererState_Disabled);

	return true;
}

Anvil::Client::Rendering::RendererState WebRenderer::GetState()
{
	return m_State;
}

bool WebRenderer::SetState(Anvil::Client::Rendering::RendererState p_State)
{
	// If the state is already set, go away
	if (m_State == p_State)
		return false;

	// Bounds checking
	if (m_State >= RendererState_Count || m_State < RendererState_Disabled)
		return false;

	m_State = p_State;

	return true;
}

bool WebRenderer::ShowNotification(std::string p_Title, std::string p_Message)
{
	return ExecuteJavascript(str(boost::format("ShowNotification(\"%s\", \"%s\");") % p_Title.c_str() % p_Message.c_str()));
}