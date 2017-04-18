#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum KeyEventType : uint32_t
	{
		eKeyEventTypeDown, // A key was pressed.
		eKeyEventTypeUp,   // A key was released.
		eKeyEventTypeChar  // A character was typed.
	};
}