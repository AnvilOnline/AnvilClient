#pragma once
#include <Interfaces/IInitializable.hpp>
#include <Blam/Math/RealColorRGB.hpp>

namespace AnvilEldorado::Rendering
{
	namespace Globals
	{
		struct Bloom;
		struct Cinematic;
		struct DepthOfField;
		struct Graphics;
	}

	class Graphics final : public AnvilCommon::IInitializable
	{
	protected:
		Globals::Bloom* GetBloomGlobals();
		Globals::Cinematic* GetCinematicGlobals();
		Globals::DepthOfField* GetDepthOfFieldGlobals();
		Globals::Graphics* GetGraphicsGlobals();

		static void OnResolutionChange();

		typedef int(*unknown_t)();
		static unknown_t unknown;

	public:
		bool Init() override;
		
		float GetRedHue();
		void SetRedHue(float p_RedHue);

		float GetGreenHue();
		void SetGreenHue(float p_GreenHue);

		float GetBlueHue();
		void SetBlueHue(float p_BlueHue);

		Blam::Math::RealColorRGB GetHue();
		void SetHue(Blam::Math::RealColorRGB &p_Hue);

		float GetSaturation();
		void SetSaturation(float p_Saturation);

		float GetBloomIntensity();
		void SetBloomIntensity(float p_Intensity);

		float GetDepthOfFieldIntensity();
		void SetDepthOfFieldIntensity(float p_Intensity);

		bool IsLetterboxEnabled();
		void SetLetterboxEnabled(bool p_Enabled = true);
	};
}