#include "EngineImpl.hpp"

#include <Globals.hpp>
#include <Utils/Logger.hpp>

#include <Game\GameImpl.hpp>
#include <Audio\AudioImpl.hpp>

#include <Psapi.h>
#include <MinHook.h>

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
	m_Game = std::make_shared<Game::GameImpl>();
	if (!m_Game->Init())
		WriteLog("Could not initialize GameImpl.");

	// Initialize audio subsystem
	m_Audio = std::make_shared<Audio::AudioImpl>();
	if (!m_Audio->Init())
		WriteLog("Could not initialize AudioImpl.");

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
	auto s_Address = ExecutableBase() + 0x200990;
	HookFunctionOffset(s_Address, LoadBinkVideo);
}

HWND __stdcall EngineImpl::hk_CreateWindowExA(DWORD p_ExStyle, LPCSTR p_ClassName, LPCSTR p_WindowName, DWORD p_Style, int p_X, int p_Y, int p_Width, int p_Height, HWND p_Parent, HMENU p_Menu, HINSTANCE p_Instance, LPVOID p_Param)
{
	return o_CreateWindowExA(p_ExStyle, p_ClassName, AnvilCommon::g_BuildInfo.c_str(), p_Style, p_X, p_Y, p_Width, p_Height, p_Parent, p_Menu, p_Instance, p_Param);
}

bool __cdecl EngineImpl::hk_LoadBinkVideo(void*, void*)
{
	// Disable bink videos
	return false;
}



EngineImpl::CreateWindowExA_t EngineImpl::o_CreateWindowExA = nullptr;
EngineImpl::LoadBinkVideo_t EngineImpl::o_LoadBinkVideo = nullptr;