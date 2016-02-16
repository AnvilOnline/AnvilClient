#include "AnvilClient.hpp"
#include <Client/Hooks/WinHooks.hpp>
#include <Client/Hooks/EngineHooks.hpp>
#include <Client/Patches/Engine/EnginePatches.hpp>
#include "Rendering/WebRenderer.hpp"

#include <Misc/BuildInfo.hpp>
#include "Functions/SDKFunctions.h"

using Anvil::Client::AnvilClient;

AnvilClient::AnvilClient() : 
	m_WinHooks(nullptr),
	m_EngineHooks(nullptr),
	m_EnginePatches(nullptr),
	m_SDKFunctions(nullptr),
	m_WindowHandle(nullptr),
	m_MapInfoBlock(nullptr),
	m_RenderingEnabled(true)
{
}

std::shared_ptr<AnvilClient> AnvilClient::GetInstance()
{
	static auto s_Instance = std::make_shared<AnvilClient>();
	return s_Instance;
}

bool AnvilClient::Init()
{
	PreInit();

	uint32_t s_BaseAddress = 0;
	uint32_t s_BaseSize = 0;

	if (Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
	{
		// This pattern allows us to find the force load map block, this has been tested on the alpha->12.x
		auto s_Address = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseSize,
			"\x56\x8B\xF1\x85\xF6\x74\x15", "xxxxxxx");

		if (s_Address)
		{
			auto s_MapBlockAddress = *reinterpret_cast<uint32_t*>(s_Address + 14);

			m_MapInfoBlock = reinterpret_cast<void*>(s_MapBlockAddress);

			auto s_MapResetBitAddress = *reinterpret_cast<uint32_t*>(s_Address + 0x25);

			m_MapResetBit = reinterpret_cast<uint16_t*>(s_MapResetBitAddress);

			WriteLog("Block Address %p, Reset Bit %p.", s_MapBlockAddress, s_MapResetBitAddress);
		}
		else
			WriteLog("Could not find force-load map info block.");
	}

	PostInit();

	return true;
}

bool AnvilClient::PreInit()
{
	// TODO: Move the SDK assignments somewhere else
	m_SDKFunctions = std::make_unique<Functions::SDKFunctions>();
	if (m_SDKFunctions)
		m_SDKFunctions->Init();

	// Generate version string
	std::stringstream s_Stream;
	s_Stream << "AnvilOnline Alpha Build: " << __DATE__ << " - " << ANVIL_BUILD;
	m_Version = s_Stream.str();

	// Set up our windows hooks, this will allow us to hook the window creation and set up for the dx hooks
	m_WinHooks = std::make_unique<Hooks::WinHooks>();
	if (m_WinHooks)
		m_WinHooks->Init();

	// Next we will hook engine related functions
	m_EngineHooks = std::make_unique<Hooks::EngineHooks>();
	if (m_EngineHooks)
		m_EngineHooks->Init();

	// Then we will apply memory patches that have to be done in inline assembly
	m_EnginePatches = std::make_unique<Patches::EnginePatches>();
	if (m_EnginePatches)
		m_EnginePatches->Init();

	return true;
}

bool AnvilClient::PostInit()
{
	auto s_ArgCount = 0;
	auto s_Args = Utils::Util::CommandLineToArgvA(GetCommandLineA(), &s_ArgCount);

	// TODO: Command line argument->function manager if this gets too large
	for (auto i = 0; i < s_ArgCount; ++i)
	{
		auto l_Argument = s_Args[i];
		if (!_strcmpi(l_Argument, "-norenderer"))
			m_RenderingEnabled = false;

		if (!_strcmpi(l_Argument, "-sleep"))
		{
			WriteLog("Sleeping for 10s.");
			Sleep(10000);
		}
	}

	// This is some superhacks right here.
	// This kick-starts all threads that have been created if they haven't already (aka this thread should be the only one running.)
	Utils::Util::ResumeAllThreads();

	return true;
}

void* AnvilClient::GetMapInfoBlock()
{
	return m_MapInfoBlock;
}

// TODO: Use engine enums for the engine game mode and game type
bool AnvilClient::ForceLoadMap(std::string p_MapName, int32_t p_GameEngineMode, int32_t p_GameType)
{
	if (!m_MapInfoBlock || !m_MapResetBit)
		return false;

	WriteLog("Force Loading %s %d %d.", p_MapName.c_str(), p_GameEngineMode, p_GameType);

	auto s_MapNameAddress = reinterpret_cast<char*>(m_MapInfoBlock) + 0x24;
	auto s_GameTypeAddress = reinterpret_cast<int32_t*>(s_MapNameAddress + 0x308);
	auto s_GameEngineModeAddress = reinterpret_cast<int32_t*>(m_MapInfoBlock);

	// Clear our the previous map name
	memset(s_MapNameAddress, 0, 32);

	// Copy over our new map name
	strncpy_s(s_MapNameAddress, 32, p_MapName.c_str(), p_MapName.size());

	// Set our gametype and engine mode
	*s_GameTypeAddress = p_GameType;
	*s_GameEngineModeAddress = p_GameEngineMode;

	// Map reload :)
	*m_MapResetBit = 1;

	// Hold off a second to allow the engine to catch up
	Sleep(150);

	return true;
}

bool AnvilClient::IsRenderingEnabled()
{
	return m_RenderingEnabled;
}

std::string AnvilClient::GetVersion()
{
	return m_Version;
}

bool AnvilClient::Shutdown()
{
	m_RenderingEnabled = false;

	if (!Rendering::WebRenderer::GetInstance()->Shutdown())
		WriteLog("WebRenderer failed to shut down properly.");

	TerminateProcess(GetCurrentProcess(), 0);

	return true;
}

void* AnvilClient::GetWindowHandle()
{
	return m_WindowHandle;
}

void AnvilClient::SetWindowHandle(void* p_Window)
{
	m_WindowHandle = p_Window;
}