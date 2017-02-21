#include "Engine.hpp"

#include <Globals.hpp>
#include <Utils/Logger.hpp>

#include <Psapi.h>
#include <MinHook.h>

using namespace AnvilEldorado;

Engine::Engine() :
	m_ModuleBase(nullptr),
	m_ModuleSize(0)
{

}

Engine::~Engine()
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

bool Engine::Init()
{
	// Disable Windows DPI Scaling
	SetProcessDPIAware();

	// Create and initialize all hooks
	CreateHooks();

	return true;
}

uint8_t* Engine::ExecutableBase()
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

size_t Engine::ExecutableSize()
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

void Engine::CreateHooks()
{
	// CreateWindowExA hook
	if (MH_CreateHookApi(L"User32", "CreateWindowExA", &hk_CreateWindowExA, reinterpret_cast<LPVOID*>(&o_CreateWindowExA)) != MH_OK)
		WriteLog("Could not hook CreateWindowExA.");

	// Bink Video Hook
	auto s_Address = ExecutableBase() + 0x200990;
	HookFunctionOffset(s_Address, LoadBinkVideo);

	// Saber initialize
	s_Address = ExecutableBase() + 0x699120;
	HookFunctionOffset(s_Address, Game_InitHalo3);

	MH_EnableHook(MH_ALL_HOOKS);
}

HWND __stdcall Engine::hk_CreateWindowExA(DWORD p_ExStyle, LPCSTR p_ClassName, LPCSTR p_WindowName, DWORD p_Style, int p_X, int p_Y, int p_Width, int p_Height, HWND p_Parent, HMENU p_Menu, HINSTANCE p_Instance, LPVOID p_Param)
{
	return o_CreateWindowExA(p_ExStyle, p_ClassName, AnvilCommon::g_BuildInfo.c_str(), p_Style, p_X, p_Y, p_Width, p_Height, p_Parent, p_Menu, p_Instance, p_Param);
}

bool __cdecl Engine::hk_LoadBinkVideo(void*, void*)
{
	// Disable bink videos
	return false;
}

void* Engine::hk_Game_InitHalo3()
{
	const auto Game_InitAudioSystem = reinterpret_cast<bool(*)()>(0x64E190);

	// Initialize Audio
	Game_InitAudioSystem();

	return nullptr;
}

int __cdecl Engine::hk_OnTagsLoaded(char* p_TagType)
{
	return 1;
}