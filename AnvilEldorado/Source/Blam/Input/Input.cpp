#include "Input.hpp"

namespace Blam::Input
{
	void BlockInput(InputType p_Type, const bool p_Block)
	{
		reinterpret_cast<uint8_t(*)(InputType, bool)>(0x512530)(p_Type, p_Block);
	}
}