#include "Client.hpp"
#include <Windows.h>
#include <Utils/Util.hpp>

using namespace Anvil::Client;

std::shared_ptr<AnvilClient> AnvilClient::GetInstance()
{
	static auto s_Instance = std::make_shared<AnvilClient>();
	return s_Instance;
}

bool AnvilClient::Init()
{
	// Disable Windows DPI Scaling
	SetProcessDPIAware();

	Utils::Util::ResumeAllThreads();

	return true;
}
