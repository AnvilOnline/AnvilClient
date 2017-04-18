#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum InputType : uint32_t
	{
		eInputTypeUi,      // ABXY, mouse clicks, etc.
		eInputTypeGame,    // All in-game actions (including camera)
						   //   Disabled when the pause menu is open
		eInputTypeSpecial, // Escape, tab, menu navigation

		eInputType_Count,
	};
}