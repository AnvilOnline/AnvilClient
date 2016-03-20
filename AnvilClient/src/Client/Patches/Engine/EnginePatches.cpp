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
		WriteLog("Could not get base information, all engine patches disabled.");
		return false;
	}

	// fmod_wasapi_fix - Universal Version
	auto s_FmodWasapiFix = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseSize, "\x6A\x00\xFF\x36\x8B\x08\x50\xFF\x51\x10\x85\xC0", "xxxxxxxxxxxx");
	if (s_FmodWasapiFix)
	{
		auto s_Ret = Utils::Util::PatchAddressInMemory(s_FmodWasapiFix, "\x6A\x02", 2);

		WriteLog("FMOD WASAPI Output patch %s.", (s_Ret ? "applied" : "not applied"));
	}
	else
		WriteLog("could not find wasapi fmod fix.");

	return true;
}
