#include "SDKFunctions.h"
#include <SDK/Network/network_session.hpp>
#include <Utils/Util.hpp>
#include <Misc/Macros.hpp>
#include <SDK/Network/network_observer.hpp>

using namespace Anvil::Client::Functions;

bool SDKFunctions::Init()
{
	uint32_t s_BaseAddress = 0;
	uint32_t s_BaseSize = 0;

	if (!Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
		return false;

	return true;
}
