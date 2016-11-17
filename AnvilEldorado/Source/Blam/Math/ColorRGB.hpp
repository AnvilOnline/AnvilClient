#pragma once
#include <cstdint>

namespace Blam::Math
{
	struct ColorRGB
	{
		uint8_t : 8;
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;

		ColorRGB();
		ColorRGB(const uint8_t red, const uint8_t green, const uint8_t blue);

		bool operator==(const ColorRGB &other) const;
		bool operator!=(const ColorRGB &other) const;
	};
}