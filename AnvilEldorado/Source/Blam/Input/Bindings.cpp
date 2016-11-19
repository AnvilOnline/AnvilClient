#include "Bindings.hpp"

namespace Blam::Input
{
	bool GetBindings(int p_LocalPlayerIndex, BindingsTable *p_Result)
	{
		if (p_LocalPlayerIndex < 0 || p_LocalPlayerIndex >= 4)
			return false;

		reinterpret_cast<void(*)(int, BindingsTable *)>(0x60BE70)(p_LocalPlayerIndex, p_Result);

		return true;
	}

	bool SetBindings(int p_LocalPlayerIndex, const BindingsTable &p_NewBindings)
	{
		if (p_LocalPlayerIndex < 0 || p_LocalPlayerIndex >= 4)
			return false;

		reinterpret_cast<void(*)(int, const BindingsTable &)>(0x60D830)(p_LocalPlayerIndex, p_NewBindings);

		return true;
	}
}