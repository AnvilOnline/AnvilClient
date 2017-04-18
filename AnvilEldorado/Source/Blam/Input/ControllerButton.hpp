#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum ControllerButton : uint8_t
	{
		eControllerButtonLeftTrigger,
		eControllerButtonRightTrigger,
		eControllerButtonDpadUp,
		eControllerButtonDpadDown,
		eControllerButtonDpadLeft,
		eControllerButtonDpadRight,
		eControllerButtonStart,
		eControllerButtonSelect,
		eControllerButtonLeftStick,
		eControllerButtonRightStick,
		eControllerButtonA,
		eControllerButtonB,
		eControllerButtonX,
		eControllerButtonY,
		eControllerButtonLeftBumper,
		eControllerButtonRightBumper,

		eControllerButton_Count,
		eControllerButton_None = 0xFF, // An invalid controller button (for use in unset bindings)
	};
}