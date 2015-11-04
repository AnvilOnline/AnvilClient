#include "WebUI.hpp"
#include "WebUISchemeHandlerFactory.hpp"

#include <include/cef_origin_whitelist.h>

using namespace Anvil::Client::Rendering;

WebUI* WebUI::m_Instance = nullptr;

WebUI* WebUI::GetInstance()
{
	if (!m_Instance)
		m_Instance = new WebUI();

	return m_Instance;
}

void WebUI::DestroyInstance()
{
	if (!m_Instance)
		return;

	delete m_Instance;
	m_Instance = nullptr;
}

WebUI::WebUI() :
	m_Shutdown(true),
	m_Frame(nullptr),
	m_Initialized(false),
	m_SchemeHandlerFactory(nullptr),
	m_MouseEnabled(false),
	m_KeyboardEnabled(false),
	m_HasDoneIntro(false),
	m_HasConsole(false)
{
}

WebUI::~WebUI()
{
	Shutdown();
}

void WebUI::Init(bool p_Debug)
{
	if (!m_Shutdown)
		return;

	m_Shutdown = false;
	m_HasDoneIntro = false;

	if (m_Frame)
		delete m_Frame;

	if (m_App.get())
		delete m_App.get();

	if (m_SchemeHandlerFactory)
		delete m_SchemeHandlerFactory;

	m_SchemeHandlerFactory = new WebUISchemeHandlerFactory();

	m_App = new WebUIApp();

	CefMainArgs s_Args(GetModuleHandle(NULL));

	auto s_Error = CefExecuteProcess(s_Args, m_App.get(), nullptr);

	if (s_Error >= 0)
		TerminateProcess(GetCurrentProcess(), 0);

	CefSettings s_Settings;

	s_Settings.multi_threaded_message_loop = true;
	CefString(&s_Settings.product_version) = "AnvilOnline";
	//CefString(&s_Settings.browser_subprocess_path) = "webui.exe";
	s_Settings.no_sandbox = true;
	s_Settings.pack_loading_disabled = false;
	s_Settings.windowless_rendering_enabled = false;
	s_Settings.ignore_certificate_errors = true;
	s_Settings.log_severity = LOGSEVERITY_DISABLE;
	s_Settings.single_process = false;

	if (p_Debug)
	{
		s_Settings.remote_debugging_port = 8884;
	}

	if (!CefInitialize(s_Args, s_Settings, m_App.get(), nullptr))
		ExitProcess(0);

	CefRegisterSchemeHandlerFactory("vu", "", m_SchemeHandlerFactory);

	CefAddCrossOriginWhitelistEntry("file://", "http", "", true);
}

void WebUI::Shutdown()
{
	if (m_Shutdown)
		return;

	m_Shutdown = true;

	if (m_Frame)
		delete m_Frame;

	if (m_App.get())
		delete m_App.get();

	if (m_SchemeHandlerFactory)
		delete m_SchemeHandlerFactory;

	CefShutdown();
}

void WebUI::Update()
{
	if (!m_Initialized)
		return;

	if (m_Frame)
		m_Frame->Update();
}

void WebUI::Render()
{
	if (!m_Initialized)
		return;

	if (m_Frame)
		m_Frame->Render();
}

void WebUI::CreateFrame(unsigned long p_Width, unsigned long p_Height, LPDIRECT3DDEVICE9 p_Device, CefMessageRouterBrowserSide::Handler* p_Handler)
{
	if (m_Initialized || m_Frame)
		return;

	m_Initialized = true;

	m_Frame = new WebUIFrame(true, p_Width, p_Height, p_Handler);
	m_Frame->Init("vu://main", p_Device);
	m_Frame->ShouldRender(false);
}

void WebUI::OnMouseEvent(unsigned long p_Width, unsigned long p_Height, unsigned int p_MouseIndex)
{
	auto s_Button = MBT_LEFT;

	CefMouseEvent s_Event;
	s_Event.x = static_cast<int32_t>(p_Width);
	s_Event.y = static_cast<int32_t>(p_Height);

	// TODO: Re-implement
}

void WebUI::OnKeyboardEvent(CefKeyEvent p_Event)
{
	if (m_Frame && m_Frame->Client().get() && m_Frame->Client()->Browser().get() != nullptr && m_Frame->ShouldRender())
		m_Frame->Client()->Browser()->GetHost()->SendKeyEvent(p_Event);
}

void WebUI::EnableMouseInput()
{
	m_MouseEnabled = true;
}

void WebUI::EnableKeyboardInput()
{
	m_KeyboardEnabled = true;
}

void WebUI::DisableMouseInput()
{
	m_MouseEnabled = false;
}

void WebUI::DisableKeyboardInput()
{
	m_KeyboardEnabled = false;
}

void WebUI::InitKeyboardHandler(HINSTANCE p_Instance, HWND p_WindowHandle)
{
}


void WebUI::ExecuteJavascript(const std::string& p_Code)
{
	if (m_Frame)
		m_Frame->ExecuteJavascript(p_Code);
}

void WebUI::ToggleConsole()
{
	m_HasConsole = !m_HasConsole;
}