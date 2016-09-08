#include "EngineHooks.hpp"
#include <stdint.h>

using Anvil::Client::Hooks::EngineHooks;

DeclareFunctionValue(EngineHooks, GameTick);
DeclareFunctionValue(EngineHooks, VerifyMapSignature);
DeclareFunctionValue(EngineHooks, StartCountdownTimer);
DeclareFunctionValue(EngineHooks, PrintDebug);
DeclareFunctionValue(EngineHooks, sub_7E6630);

bool EngineHooks::Init()
{
	uint32_t s_BaseAddress = 0;
	uint32_t s_BaseSize = 0;

	if (!Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
	{
		WriteLog(L"Could not get base information, all engine hooks disabled.");
		return false;
	}

	WriteLog(L"EngineHooks init.");
	return true;
}