#include "Client.hpp"
#include <Windows.h>

using namespace Anvil::Client;

bool AnvilClient::Init()
{
	// Disable Windows DPI Scaling
	SetProcessDPIAware();

	return false;
}
