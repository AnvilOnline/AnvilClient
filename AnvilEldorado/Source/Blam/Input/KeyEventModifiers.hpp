#pragma once
#include <cstdint>

namespace AnvilEldorado::Blam::Input
{
	enum class KeyEventModifiers : uint8_t
	{
		eKeyEventModifiersShift = 1 << 0,
		eKeyEventModifiersCtrl = 1 << 1,
		eKeyEventModifiersalt = 1 << 2,
	};
}