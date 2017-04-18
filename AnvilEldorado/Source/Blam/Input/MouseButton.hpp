#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum MouseButton : uint8_t
	{
		eMouseButtonLeft,
		eMouseButtonMiddle,
		eMouseButtonRight,
		eMouseButton4,
		eMouseButton5,

		// Not sure what these 3 are...they aren't buttons and can't be bound to anything
		eMouseButtonUnk5,
		eMouseButtonUnk6,
		eMouseButtonUnk7,

		eMouseButtonWheelUp,
		eMouseButtonWheelDown,

		eMouseButton_Count,
		eMouseButton_None = 0xFF, // An invalid mouse button (for use in unset bindings)
	};
}