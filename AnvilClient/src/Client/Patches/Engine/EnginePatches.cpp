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

	// tag edit patch 1
	auto s_TagPatch1 = Utils::Util::FindPattern(reinterpret_cast<void *>(s_BaseAddress), s_BaseSize, "\x84\xC0\x75\xFF\x8D\x85\xFF\xFE\xFF\xFF\x68", "xxx?xx?xxxx");

	if (s_TagPatch1)
	{
		auto s_Ret = Utils::Util::PatchAddressInMemory(s_TagPatch1 + 2, "\xEB", 1);

		WriteLog("Tag Patch 1 %s.", (s_Ret ? "applied" : "not applied"));
	}
	else
		WriteLog("could not find tag patch 1 fix.");

	// tag edit patch 2
	auto s_TagPatch2 = Utils::Util::FindPattern(reinterpret_cast<void *>(s_BaseAddress), s_BaseSize, "\x83\xC4\x0C\x3B\x03\x75", "xxxxxx");

	if (s_TagPatch2)
	{
		auto s_Ret = Utils::Util::PatchAddressInMemory(s_TagPatch2 + 5, "\x90\x90", 2);

		WriteLog("Tag Patch 2 %s.", (s_Ret ? "applied" : "not applied"));
	}
	else
		WriteLog("could not find tag patch 2 fix.");

	// tag edit patch 3
	auto s_TagPatch3 = Utils::Util::FindPattern(reinterpret_cast<void *>(s_BaseAddress), s_BaseSize, "\xC6\x86\x90\x33\x00\x00\x01", "xxxxxxx");

	if (s_TagPatch3)
	{
		auto s_Ret = Utils::Util::PatchAddressInMemory(s_TagPatch3 - 2, "\x90\x90", 2);

		WriteLog("Tag Patch 3 %s.", (s_Ret ? "applied" : "not applied"));
	}
	else
		WriteLog("could not find tag patch 3 fix.");

	return true;
}
