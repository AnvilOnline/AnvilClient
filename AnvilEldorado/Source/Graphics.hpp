#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Module.hpp"
#include "Blam\Math\RealColorRGB.hpp"

namespace AnvilEldorado
{
	struct GraphicsGlobals
	{
		int32_t Override; // bool
		float Saturation;
		Blam::Math::RealColorRGB Hue;
	};

	struct BloomGlobals
	{
		int32_t Enabled; // bool
		float Intensity;
	};

	struct DepthOfFieldGlobals
	{
		int32_t Enabled; // bool
		float Intensity;
	};

	struct CinematicGlobals
	{
		int32_t : 32; // Unknown
		int32_t LetterboxEnabled;
	};

	class Graphics final : public Module, public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Graphics>
	{
	public:
		Command *VarSaturation;

		// TODO: possibly refactor into single #RRGGBB command or provide such functionality as a separate all-in-one command
		Command *VarRedHue;
		Command *VarGreenHue;
		Command *VarBlueHue;

		Command *VarBloom;
		Command *VarDepthOfField;
		Command *VarLetterbox;

		Command *VarUIScaling;

		static GraphicsGlobals *GetGraphicsGlobals();
		static BloomGlobals *GetBloomGlobals();
		static DepthOfFieldGlobals *GetDepthOfFieldGlobals();
		static CinematicGlobals *GetCinematicGlobals();

		Graphics();

		bool Init() override;
	};
}