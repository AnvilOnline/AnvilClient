#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum class UiButtonCode : int32_t
	{
		A,
		B,
		X,
		Y,
		RB,
		LB,
		LT,
		RT,
		DpadUp,
		DpadDown,
		DpadLeft,
		DpadRight,
		Start,
		Back,
		LS,
		RS,
		EmulatedInput,
		Unk1,
		Left,
		Right,
		Up,
		Down,
		LSHorizontal = 0x1A,
		LSVertical,
		RSHorizontal,
		RSVertical
	};
}