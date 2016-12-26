#pragma once

#include "Interfaces\IInitializable.hpp"

#include "Utils\Singleton.hpp"

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
		int32_t Unknown0;
		int32_t LetterboxEnabled;
	};

	class Graphics final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Graphics>
	{
	public:
		static GraphicsGlobals *GetGraphicsGlobals();
		static BloomGlobals *GetBloomGlobals();
		static DepthOfFieldGlobals *GetDepthOfFieldGlobals();
		static CinematicGlobals *GetCinematicGlobals();

		bool Init() override;

		float GetRedHue() const;
		void SetRedHue(const float p_RedHue);

		float GetGreenHue() const;
		void SetGreenHue(const float p_GreenHue);

		float GetBlueHue() const;
		void SetBlueHue(const float p_BlueHue);

		Blam::Math::RealColorRGB GetHue() const;
		void SetHue(const Blam::Math::RealColorRGB &p_Hue);

		float GetSaturation() const;
		void SetSaturation(const float p_Saturation);

		float GetBloomIntensity() const;
		void SetBloomIntensity(const float p_Intensity);

		float GetDepthOfFieldIntensity() const;
		void SetDepthOfFieldIntensity(const float p_Intensity);

		bool IsLetterboxEnabled() const;
		void SetLetterboxEnabled(const bool p_Enabled = true);
	};
}