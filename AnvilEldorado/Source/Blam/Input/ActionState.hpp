#pragma once
#include <cstdint>

namespace AnvilEldorado::Blam::Input
{
	struct ActionState
	{
		uint16_t Msec;     // Number of milliseconds the action has been down for
		uint8_t Ticks;     // Number of ticks the action has been down for
		uint8_t Flags;     // See the ActionStateFlags enum
		float Unknown4;
		uint8_t Unknown8;
		uint8_t Unknown9;
		uint16_t UnknownA; // Padding?
	};

#if _DEBUG
	static_assert(sizeof(ActionState) == 0xC, "Invalid ActionState size");
#endif
}