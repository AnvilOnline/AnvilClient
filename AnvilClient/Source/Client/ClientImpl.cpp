#include "ClientImpl.hpp"
#include <EngineImpl.hpp>

#include <Windows.h>
#include <Utils/Util.hpp>
#include <Utils/Logger.hpp>

#include <MinHook.h>

using namespace AnvilCommon;
using namespace Anvil::Client;

#ifdef ANVIL_DEW
#pragma comment(lib, "AnvilEldorado")
#endif

#ifdef ANVIL_AUSAR
#pragma comment(lib, "AnvilAusar")
#endif

extern std::shared_ptr<AnvilCommon::Client> GetClientInterface()
{
	return ClientImpl::GetInstance();
}

std::shared_ptr<ClientImpl> ClientImpl::GetInstance()
{
	static auto s_Instance = std::make_shared<ClientImpl>();
	return s_Instance;
}

bool ClientImpl::Init()
{
	// Initialize MinHook
	if (MH_Initialize() != MH_OK)
	{
		WriteLog("Failed to initialize libMinHook.");
		return false;
	}

#ifdef ANVIL_DEW
	// Initialize Eldorado
	m_Engine = std::shared_ptr<AnvilCommon::Engine>(new AnvilEldorado::EngineImpl);
#endif

#ifdef ANVIL_AUSAR
	// Initialize Ausar
	m_Engine = std::shared_ptr<AnvilCommon::IEngineInitializable>(new AnvilAusar::Engine);
#endif

	if (!m_Engine->Init())
	{
		WriteLog("Failed to initialize engine.");
		return false;
	}

#if defined(_DEBUG) && defined(ANVIL_DEW)
	WriteLog("Sleeping for 5s.");
	Sleep(15000);
#endif

	Utils::Util::ResumeAllThreads();

	return true;
}

std::shared_ptr<AnvilCommon::Engine> ClientImpl::GetEngine()
{
	return m_Engine;
}