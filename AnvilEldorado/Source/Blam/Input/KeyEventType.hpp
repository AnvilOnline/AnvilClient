#pragma once
#include <cstdint>

namespace AnvilEldorado::Blam::Input
{
	enum class KeyEventType : uint32_t
	{
		eKeyEventTypeDown, // A key was pressed.
		eKeyEventTypeUp,   // A key was released.
		eKeyEventTypeChar  // A character was typed.
	};
}