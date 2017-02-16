#pragma once

namespace AnvilEldorado::Rendering::Globals
{
	struct alignas(4) DepthOfField
	{
		bool Enabled;
		float Intensity;
	};
}