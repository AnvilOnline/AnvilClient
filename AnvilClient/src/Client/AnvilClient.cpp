#include "AnvilClient.hpp"
#include <Client/Hooks/WinHooks.hpp>
#include <Client/Hooks/EngineHooks.hpp>
#include <Client/Patches/Engine/EnginePatches.hpp>
#include "Rendering/WebRenderer.hpp"

#include <Misc/BuildInfo.hpp>

using Anvil::Client::AnvilClient;

AnvilClient* AnvilClient::m_Instance = nullptr;

AnvilClient::AnvilClient() : 
	m_WinHooks(nullptr),
	m_EngineHooks(nullptr),
	m_EnginePatches(nullptr),
	m_MapInfoBlock(nullptr)
{
}

AnvilClient::~AnvilClient()
{
}

AnvilClient* AnvilClient::GetInstance()
{
	if (!m_Instance)
		m_Instance = new AnvilClient;

	return m_Instance;
}

bool AnvilClient::Init()
{
	PreInit();

	unsigned long s_BaseAddress = 0;
	unsigned long s_BaseSize = 0;

	if (Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
	{
		auto s_Address = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseSize,
			"\x56\x8B\xF1\x85\xF6\x74\x15", "xxxxxxx");

		if (s_Address)
		{
			auto s_MapBlockAddress = *reinterpret_cast<unsigned long*>(s_Address + 14);

			m_MapInfoBlock = reinterpret_cast<void*>(s_MapBlockAddress);

			auto s_MapResetBitAddress = *reinterpret_cast<unsigned long*>(s_Address + 0x25);

			m_MapResetBit = reinterpret_cast<short*>(s_MapResetBitAddress);

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
	// Generate version string
	std::stringstream s_Stream;
	s_Stream << "AnvilOnline Alpha Build: " << __DATE__ << "-" << ANVIL_BUILD;
	m_Version = s_Stream.str();

	auto s_ArgCount = 0;
	auto s_Args = Utils::Util::CommandLineToArgvA(GetCommandLineA(), &s_ArgCount);

	for (auto i = 0; i < s_ArgCount; ++i)
	{
		auto l_Argument = s_Args[i];
		if (!_strcmpi(l_Argument, "-norenderer"))
			Rendering::WebRenderer::GetInstance()->SetState(Rendering::RendererState_Disabled);

		if (!_strcmpi(l_Argument, "-sleep"))
		{
			WriteLog("Sleeping for 10s.");
			Sleep(10000);
		}
	}

	m_WinHooks = new Hooks::WinHooks;
	if (m_WinHooks)
		m_WinHooks->Init();

	m_EngineHooks = new Hooks::EngineHooks;
	if (m_EngineHooks)
		m_EngineHooks->Init();


	m_EnginePatches = new Patches::EnginePatches;
	if (m_EnginePatches)
		m_EnginePatches->Init();

	return true;
}

bool AnvilClient::PostInit()
{
	
	return true;
}

void* AnvilClient::GetMapInfoBlock()
{
	return m_MapInfoBlock;
}

bool AnvilClient::ForceLoadMap(std::string p_MapName, int p_GameEngineMode, int p_GameType)
{
	if (!m_MapInfoBlock || !m_MapResetBit)
		return false;

	WriteLog("Force Loading %s %d %d.", p_MapName.c_str(), p_GameEngineMode, p_GameType);

	auto s_MapNameAddress = reinterpret_cast<char*>(m_MapInfoBlock) + 0x24;
	auto s_GameTypeAddress = reinterpret_cast<int*>(s_MapNameAddress + 0x308);
	auto s_GameEngineModeAddress = reinterpret_cast<int*>(m_MapInfoBlock);

	memset(s_MapNameAddress, 0, 32);
	strncpy_s(s_MapNameAddress, 32, p_MapName.c_str(), p_MapName.size());

	*s_GameTypeAddress = p_GameType;
	*s_GameEngineModeAddress = p_GameEngineMode;

	*m_MapResetBit = 1;

	Sleep(250);

	return true;
}

std::string AnvilClient::GetVersion()
{
	return m_Version;
}