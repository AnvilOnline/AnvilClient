#pragma once
#include <cstdint>

namespace AnvilEldorado::Blam::Input
{
	enum class InputType : uint32_t
	{
		eInputTypeUi,      // ABXY, mouse clicks, etc.
		eInputTypeGame,    // All in-game actions (including camera)
						   //   Disabled when the pause menu is open
		eInputTypeSpecial, // Escape, tab, menu navigation
		eInputType_Count,
	};
}