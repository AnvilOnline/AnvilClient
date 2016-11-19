#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum class ControllerButton : int8_t
	{
		None = -1,
		LeftTrigger,
		RightTrigger,
		DpadUp,
		DpadDown,
		DpadLeft,
		DpadRight,
		Start,
		Select,
		LeftStick,
		RightStick,
		A,
		B,
		X,
		Y,
		LeftBumper,
		RightBumper,

		Count
	};
}