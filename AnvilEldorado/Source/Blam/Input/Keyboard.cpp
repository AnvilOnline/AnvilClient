#include "Keyboard.hpp"

namespace Blam::Input
{
	bool KeyEvent::Read(const InputType &p_Type)
	{
		return reinterpret_cast<bool(*)(KeyEvent*, InputType)>(0x5118C0)(this, p_Type);
	}

	uint8_t GetKeyTicks(const KeyCode &p_Key, const InputType &p_Type)
	{
		return reinterpret_cast<uint8_t(*)(KeyCode, InputType)>(0x511B60)(p_Key, p_Type);
	}

	uint16_t GetKeyMs(const KeyCode &p_Key, const InputType &p_Type)
	{
		return reinterpret_cast<uint8_t(*)(KeyCode, InputType)>(0x511CE0)(p_Key, p_Type);
	}
}