#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum class InputType : int32_t
	{
		None = -1,
		Ui,
		Game,
		Special,

		Count
	};

	void BlockInput(InputType p_Type, const bool p_Block);
}