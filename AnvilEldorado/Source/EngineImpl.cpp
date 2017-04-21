#include "EngineImpl.hpp"

#include <Globals.hpp>
#include <Utils/Logger.hpp>

#include <Psapi.h>
#include <MinHook.h>

#include <Game\Audio\AudioImpl.hpp>
#include <Game\Input\InputImpl.hpp>
#include <Game\Networking\NetworkingImpl.hpp>
#include <Game\Players\PlayerImpl.hpp>
#include <Game\UI\UIImpl.hpp>

#include <Utils\Patch.hpp>

using namespace AnvilEldorado;

EngineImpl::EngineImpl() :
	m_ModuleBase(nullptr),
	m_ModuleSize(0)
{

}

EngineImpl::~EngineImpl()
{
	// Clear out our module base
	if (m_ModuleBase)
		m_ModuleBase = nullptr;

	// Clear out our module size
	if (m_ModuleSize)
		m_ModuleSize = 0;

	// Disable all hooks
	MH_DisableHook(MH_ALL_HOOKS);
}

bool EngineImpl::Init()
{
	// Disable Windows DPI Scaling
	SetProcessDPIAware();

	// Create and initialize all hooks
	CreateHooks();

	// Initialize game subsystem
	/*m_Game = std::make_shared<Game::GameImpl>();
	if (!m_Game->Init())
		WriteLog("Could not initialize GameImpl.");*/

	// Initialize audio subsystem
	m_Audio = std::make_shared<Game::Audio::AudioImpl>();
	if (!m_Audio->Init())
		WriteLog("Could not initialize AudioImpl.");

	// Initialize the input subsystem
	m_Input = std::make_shared<Game::Input::InputImpl>();
	if (!m_Input->Init())
		WriteLog("Could not initialize InputImpl.");

	// Initialize the networking subsystem
	m_Networking = std::make_shared<Game::Networking::NetworkingImpl>();
	if (!m_Networking->Init())
		WriteLog("Could not initalize NetworkingImpl.");

	// Initialize the player subsystem
	m_Player = std::make_shared<Game::Players::PlayerImpl>();
	if (!m_Player->Init())
		WriteLog("Could not initialize PlayerImpl.");

	// Initialize the ui subsystem
	m_UI = std::make_shared<Game::UI::UIImpl>();
	if (!m_UI->Init())
		WriteLog("Could not initialize UIImpl.");

	// Enable all hooked functions
	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

uint8_t* EngineImpl::ExecutableBase()
{
	if (m_ModuleBase)
		m_ModuleBase;

	MODULEINFO s_ModuleInfo = { 0 };

	auto s_Result = GetModuleInformation(GetCurrentProcess(),
		GetModuleHandle(nullptr),
		&s_ModuleInfo, sizeof(s_ModuleInfo));

	if (!s_Result)
		return nullptr;

	m_ModuleBase = s_ModuleInfo.lpBaseOfDll;

	return static_cast<uint8_t*>(m_ModuleBase);
}

size_t EngineImpl::ExecutableSize()
{
	if (m_ModuleSize)
		return m_ModuleSize;

	MODULEINFO s_ModuleInfo = { 0 };

	auto s_Result = GetModuleInformation(GetCurrentProcess(),
		GetModuleHandle(nullptr),
		&s_ModuleInfo, sizeof(s_ModuleInfo));

	if (!s_Result)
		return 0;

	m_ModuleSize = s_ModuleInfo.SizeOfImage;

	return m_ModuleSize;
}

void EngineImpl::CreateHooks()
{
	// CreateWindowExA hook
	if (MH_CreateHookApi(L"User32", "CreateWindowExA", &hk_CreateWindowExA, reinterpret_cast<LPVOID*>(&o_CreateWindowExA)) != MH_OK)
		WriteLog("Could not hook CreateWindowExA.");

	// Bink Video Hook
	auto s_Address = ExecutableBase() + 0x699120;
	HookFunctionOffset(s_Address, LoadBinkVideo);

	// Tag Cache Validation Hook
	s_Address = ExecutableBase() + 0x102210;
	HookFunctionOffset(s_Address, ValidateTagCache);

	// Account Processing Hook
	s_Address = ExecutableBase() + 0x4372E0;
	HookFunctionOffset(s_Address, ProcessAccountInfo);

	// Account Verification Hook
	s_Address = ExecutableBase() + 0x437360;
	HookFunctionOffset(s_Address, VerifyAccountAndLoadAnticheat);

	// Force Russian Localizationn Hook
	s_Address = ExecutableBase() + 0x2E5C0;
	HookFunctionOffset(s_Address, ForceRussianLocale);

	// Allow tag changes
	AnvilCommon::Utils::Patch::NopFill(0x102874, 2); //TODO: sub_52CCC0 == *v10 true

	//TODO: Is this needed?
	//AnvilCommon::Utils::Patch::NopFill(0x1030AA, 2); //TODO: sub_508F80 return true
}

DeclareDetouredFunction(EngineImpl, HWND, __stdcall, CreateWindowExA, DWORD p_ExStyle, LPCSTR p_ClassName, LPCSTR p_WindowName, DWORD p_Style, int p_X, int p_Y, int p_Width, int p_Height, HWND p_Parent, HMENU p_Menu, HINSTANCE p_Instance, LPVOID p_Param)
{
	return o_CreateWindowExA(p_ExStyle, p_ClassName, AnvilCommon::g_BuildInfo.c_str(), p_Style, p_X, p_Y, p_Width, p_Height, p_Parent, p_Menu, p_Instance, p_Param);
}

DeclareDetouredFunction(EngineImpl, bool, __cdecl, LoadBinkVideo, int p_VideoID, char *p_DestBuf)
{
	// Disable bink videos
	return false;
}

DeclareDetouredFunction(EngineImpl, char, __cdecl, ValidateTagCache, void *a1)
{
	// Automatically pass actual validation
	return true;
}

DeclareDetouredFunction(EngineImpl, char*, __cdecl, ProcessAccountInfo)
{
	// Ignore authentication
	return nullptr;
}

DeclareDetouredFunction(EngineImpl, char, __cdecl, VerifyAccountAndLoadAnticheat)
{
	// Ignore authentication
	return false;
}

DeclareDetouredFunction(EngineImpl, char, __cdecl, ForceRussianLocale)
{
	// Enabling reading Language.LangShort from game.cfg
	return false;
}