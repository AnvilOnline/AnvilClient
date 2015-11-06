#include "AnvilClient.hpp"
#include <Client/Hooks/WinHooks.hpp>
#include <Client/Hooks/EngineHooks.hpp>
#include <Client/Patches/Engine/EnginePatches.hpp>
#include "Rendering/WebRenderer.hpp"

using Anvil::Client::AnvilClient;

AnvilClient* AnvilClient::m_Instance = nullptr;

AnvilClient::AnvilClient() : 
	m_WinHooks(nullptr),
	m_EngineHooks(nullptr),
	m_EnginePatches(nullptr)
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

	PostInit();
	return true;
}

bool AnvilClient::PreInit()
{
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