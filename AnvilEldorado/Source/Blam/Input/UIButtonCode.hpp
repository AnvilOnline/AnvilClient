#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum UiButtonCode : uint32_t
	{
		eUiButtonCodeA = 0,
		eUiButtonCodeB,
		eUiButtonCodeX,
		eUiButtonCodeY,
		eUiButtonCodeRB,
		eUiButtonCodeLB,
		eUiButtonCodeLT,
		eUiButtonCodeRT,
		eUiButtonCodeDpadUp,
		eUiButtonCodeDpadDown,
		eUiButtonCodeDpadLeft,
		eUiButtonCodeDpadRight,
		eUiButtonCodeStart,
		eUiButtonCodeBack,
		eUiButtonCodeLS,
		eUiButtonCodeRS,
		eUiButtonCodeEmulatedInput,
		eUiButtonCodeUnk1,
		eUiButtonCodeLeft, // analog/arrow left
		eUiButtonCodeRight, // analog/arrow right
		eUiButtonCodeUp, // analog/arrow up
		eUiButtonCodeDown, // analog/arrow down
		eUiButtonCodeLSVertical = 0x1A,
		eUiButtonCodeRSVertical,
	};
}