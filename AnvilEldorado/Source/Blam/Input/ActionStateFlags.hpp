#pragma once
#include <cstdint>

namespace AnvilEldorado::Blam::Input
{
	enum ActionStateFlags : uint8_t
	{
		eActionStateFlags_None = 0,
		eActionStateFlagsHandled = 1 << 0, // Action has been handled and should be ignored
										   // TODO: Other flags?
	};
}