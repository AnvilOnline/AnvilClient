#pragma once

namespace AnvilEldorado::Blam::Input
{
	struct ConfigurableAction
	{
		const char *Name;        // "settings_" followed by the string_id
		int DefaultPrimaryKey;   // KeyCode, (MouseButton + 1) * 0x100, or -1
		int DefaultSecondaryKey; // KeyCode, (MouseButton + 1) * 0x100, or -1
		int Action;              // GameAction
	};

#if _DEBUG
	static_assert(sizeof(ConfigurableAction) == 0x10, "Invalid ConfigurableAction size");
#endif
}