#include "EnginePatches.hpp"
#include <Utils/Util.hpp>
#include <Utils/Logger.hpp>

using Anvil::Client::Patches::EnginePatches;

bool EnginePatches::Init()
{
	uint32_t s_BaseAddress = 0;
	uint32_t s_BaseSize = 0;

	if (!Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
	{
		WriteLog(L"Could not get base information, all engine patches disabled.");
		return false;
	}

	return true;
}
