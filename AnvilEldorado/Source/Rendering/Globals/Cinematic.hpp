#pragma once
#include <cstdint>

namespace AnvilEldorado::Rendering::Globals
{
	struct Cinematic
	{
		unsigned char _Unknown00[4];
		int32_t LetterboxEnabled; // TODO: Invesitigate this
	};
}