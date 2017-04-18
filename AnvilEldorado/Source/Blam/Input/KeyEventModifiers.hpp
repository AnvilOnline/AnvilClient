#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum KeyEventModifiers : uint8_t
	{
		eKeyEventModifiersShift = 1 << 0,
		eKeyEventModifiersCtrl = 1 << 1,
		eKeyEventModifiersalt = 1 << 2,
	};
}