#include "EngineHooks.hpp"

using Anvil::Client::Hooks::EngineHooks;

DeclareFunctionValue(EngineHooks, GameTick);
DeclareFunctionValue(EngineHooks, VerifyMapSignature);
DeclareFunctionValue(EngineHooks, StartCountdownTimer);

DeclareFunctionValue(EngineHooks, sub_5C7E40);

bool EngineHooks::Init()
{
	unsigned long s_BaseAddress = 0;
	unsigned long s_BaseSize = 0;

	if (!Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
	{
		WriteLog("Could not get base information, all engine hooks disabled.");
		return false;
	}

	// Hook game tick function
	DeclareHookPattern(GameTick, s_BaseAddress, s_BaseSize, "\x55\x8B\xEC\xB8\x6C\x16\x00\x00\xE8\x00\x00\x00\x00\x33\xC0\x66", "xxxxxxxxx???xxxx");

	DeclareHookAtOffset(sub_5C7E40, s_BaseAddress + 0x1C7E40);

	WriteLog("EngineHooks init success.");
	return true;
}