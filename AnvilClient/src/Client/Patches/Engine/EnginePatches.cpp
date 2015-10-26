#include "EnginePatches.hpp"
#include <Utils/Util.hpp>
#include <Utils/Logger.hpp>

using Anvil::Client::Patches::EnginePatches;

bool EnginePatches::Init()
{
	unsigned long s_baseAddress = 0;
	unsigned long s_baseSize = 0;

	if (!Utils::Util::GetExecutableInfo(s_baseAddress, s_baseSize))
	{
		WriteLog("Could not get base information, all engine patches disabled.");
		return false;
	}

	// fmod_wasapi_fix
	auto s_fmod_wasapi_fix = Utils::Util::FindPattern(reinterpret_cast<void*>(s_baseAddress), s_baseSize, "\x6A\x00\xFF\x36\x8B\x08\x50\xFF\x51\x10\x85\xC0", "xxxxxxxxxxxx");
	if (s_fmod_wasapi_fix)
	{
		auto s_ret = Utils::Util::PatchAddressInMemory(s_fmod_wasapi_fix, "\x6A\x02", 2);

		WriteLog("FMOD WASAPI Output patch %s.", (s_ret ? "applied" : "not applied"));
	}
	else
		WriteLog("could not find wasapi fmod fix.");

	// english patches
	auto s_english_fix = Utils::Util::FindPattern(reinterpret_cast<void*>(s_baseAddress), s_baseAddress, "\x83\x3D\x00\x00\x00\x01\x0B\x74\x2F\xC7\x05\x00\x00\x00\x01\x0B", "xx???xxxxxx???xx");
	if (s_english_fix)
	{
		auto s_ret = Utils::Util::PatchAddressInMemory(s_english_fix + 6, "\x00", 1);
		WriteLog("First english patch %s.", (s_ret ? "applied" : "not applied"));

		s_ret = Utils::Util::PatchAddressInMemory(s_english_fix + 15, "\x00", 1);
		WriteLog("Second english patch %s.", (s_ret ? "applied" : "not applied"));
	}
	return true;
}
