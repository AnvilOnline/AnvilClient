#include <Globals.hpp>
#include <BuildInfo.hpp>

std::string AnvilCommon::g_BuildInfo = "AnvilAusar Client - " + std::string(__DATE__) + " - Build: " + std::to_string(ANVIL_BUILD);
void* AnvilCommon::g_ThreadStorage = AnvilCommon::Internal_GetThreadStorage();

void* AnvilCommon::Internal_GetThreadStorage(const size_t p_Offset)
{
	// TODO: Implement
	return nullptr;
}