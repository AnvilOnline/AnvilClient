// 496D20
#include <Client/Hooks/EngineHooks.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(EngineHooks, int, PrintDebug, __cdecl, char* p_Source, char* p_Format, ...)
{
	auto s_Ret = 0;
	if (p_Format)
	{
		va_list s_List;
		va_start(s_List, p_Format);

		auto s_FinalLength = _vscprintf(p_Format, s_List) + 1;
		auto s_FinalString = static_cast<char*>(new char[s_FinalLength]);
		if (!s_FinalString)
			return false;

		vsprintf_s(s_FinalString, s_FinalLength, p_Format, s_List);

		if (strcmpi(p_Source, "game_tick") != 0)
			WriteLog("[%s] : %s.", p_Source, s_FinalString);

		s_Ret = o_PrintDebug(p_Source, p_Format, s_List);

		delete s_FinalString;

		va_end(s_List);
	}
	else
	{
		if (strcmpi(p_Source, "game_tick") != 0)
			WriteLog("[%s]", p_Source);

		s_Ret = o_PrintDebug(p_Source, nullptr);
	}

	return s_Ret;
}