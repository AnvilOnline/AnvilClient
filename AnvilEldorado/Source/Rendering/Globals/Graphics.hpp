#pragma once
#include <Blam/Math/RealColorRGB.hpp>

namespace AnvilEldorado::Rendering::Globals
{
	struct alignas(4) Graphics
	{
		bool Override;
		float Saturation;
		Blam::Math::RealColorRGB Hue;
	};
}