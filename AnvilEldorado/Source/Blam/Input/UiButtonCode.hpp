#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum class UiButtonCode : uint32_t
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
		eUiButtonCodeLeft,
		eUiButtonCodeRight,
		eUiButtonCodeUp,
		eUiButtonCodeDown,
		eUiButtonCodeLSHorizontal = 0x1A,
		eUiButtonCodeLSVertical,
		eUiButtonCodeRSHorizontal,
		eUiButtonCodeRSVertical
	};
}