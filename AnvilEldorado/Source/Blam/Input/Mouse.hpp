#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum class MouseButton : int8_t
	{
		None = -1,
		Left,
		Middle,
		Right,
		Button4,
		Button5,
		Unused1,
		Unused2,
		Unused3,
		WheelUp,
		WheelDown,

		Count
	};
}