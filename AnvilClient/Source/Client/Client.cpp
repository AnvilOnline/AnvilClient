#include "Client.hpp"
#include <Windows.h>
#include <Utils/Util.hpp>
#include <Utils/Logger.hpp>

#include <Engine.hpp>
#include <MinHook.h>

using namespace AnvilCommon;
using namespace Anvil::Client;

#ifdef ANVIL_DEW
#pragma comment(lib, "AnvilEldorado")
#endif

#ifdef ANVIL_AUSAR
#pragma comment(lib, "AnvilAusar")
#endif

std::shared_ptr<AnvilClient> AnvilClient::GetInstance()
{
	static auto s_Instance = std::make_shared<AnvilClient>();
	return s_Instance;
}

bool AnvilClient::Init()
{
	// Initialize MinHook
	if (MH_Initialize() != MH_OK)
	{
		WriteLog("Failed to initialize libMinHook.");
		return false;
	}

#ifdef ANVIL_DEW
	// Initialize Dewrito
	m_Engine = std::shared_ptr<AnvilCommon::IInitializable>(new AnvilEldorado::Engine);
#endif

#ifdef ANVIL_AUSAR
	// Initialize Ausar
	m_Engine = std::shared_ptr<AnvilCommon::IInitializable>(new AnvilAusar::Engine);
#endif

	if (!m_Engine->Init())
	{
		WriteLog("Failed to initialize engine.");
		return false;
	}

	Utils::Util::ResumeAllThreads();

	return true;
}
