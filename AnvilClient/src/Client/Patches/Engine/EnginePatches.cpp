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

	// Halo Online 11.x
	auto s_EnglishV11Patch = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseAddress, "\x83\x3D\x00\x00\x00\x01\x0B\x74\x2F\xC7\x05\x00\x00\x00\x01\x0B", "xx???xxxxxx???xx");
	if (s_EnglishV11Patch)
	{
		auto s_Ret = Utils::Util::PatchAddressInMemory(s_EnglishV11Patch + 6, "\x00", 1);
		WriteLog("First english patch %s.", (s_Ret ? "applied" : "not applied"));

		s_Ret = Utils::Util::PatchAddressInMemory(s_EnglishV11Patch + 15, "\x00", 1);
		WriteLog("Second english patch %s.", (s_Ret ? "applied" : "not applied"));
	}
	else
		WriteLog("English Fix Patch Failed.");

	// Halo Online 12.x
	// TODO: Signature based
	auto s_EnglishV12Patch = Utils::Util::PatchAddressInFile(0x2C73DD, "\xBB\x00", 2);
	WriteLog("English patch v2 applied %s.", (s_EnglishV12Patch ? "successfully" : "unsuccessfully"));

	return true;
}
