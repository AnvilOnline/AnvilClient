#include <sstream>

#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"

#include "Globals.hpp"
#include "Graphics.hpp"
#include "UserInterface.hpp"

namespace AnvilEldorado
{
	const auto ApplyResolutionChange = reinterpret_cast<void(__thiscall *)()>(0xA226D0);

	void ResolutionChangeHook()
	{
		ApplyResolutionChange();

		UserInterface::Instance()->ApplyResolution();
	}

	double AspectRatioHook()
	{
		auto *s_GameResolution = reinterpret_cast<int32_t *>(0x19106C0);

		return ((double)s_GameResolution[0] / (double)s_GameResolution[1]);
	}

	GraphicsGlobals *Graphics::GetGraphicsGlobals()
	{
		static GraphicsGlobals *s_GraphicsGlobals = nullptr;

		if (s_GraphicsGlobals == nullptr)
			s_GraphicsGlobals = AnvilCommon::GetThreadStorage<GraphicsGlobals>(0x3C0);

		return s_GraphicsGlobals;
	}

	BloomGlobals *Graphics::GetBloomGlobals()
	{
		static BloomGlobals *s_BloomGlobals = nullptr;

		if (s_BloomGlobals == nullptr)
			s_BloomGlobals = AnvilCommon::GetThreadStorage<BloomGlobals>(0x104);

		return s_BloomGlobals;
	}

	DepthOfFieldGlobals *Graphics::GetDepthOfFieldGlobals()
	{
		static DepthOfFieldGlobals *s_DepthOfFieldGlobals = nullptr;

		if (s_DepthOfFieldGlobals == nullptr)
			s_DepthOfFieldGlobals = AnvilCommon::GetThreadStorage<DepthOfFieldGlobals>(0x3DC);

		return s_DepthOfFieldGlobals;
	}

	CinematicGlobals *Graphics::GetCinematicGlobals()
	{
		static CinematicGlobals *s_CinematicGlobals = nullptr;

		if (s_CinematicGlobals == nullptr)
			s_CinematicGlobals = AnvilCommon::GetThreadStorage<CinematicGlobals>(0x324);

		return s_CinematicGlobals;
	}

	bool Graphics::Init()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Runs when the game's resolution is changed
		return Hook(0x621303, ResolutionChangeHook, HookFlags::IsCall).Apply()
			// Fix aspect ratio not matching resolution
			&& Hook(0x6648C9, AspectRatioHook, HookFlags::IsCall).Apply()
			&& Hook(0x216487, AspectRatioHook, HookFlags::IsCall).Apply()
			// Disable converting the game's resolution to 16:9
			&& Patch::NopFill(0x62217D, 2)
			&& Patch::NopFill(0x622183, 6)
			// Allow the user to select any resolution that Windows supports in the settings screen
			&& Patch::NopFill(0x10BF1B, 2)
			&& Patch::NopFill(0x10BF21, 6);
	}

	float Graphics::GetRedHue() const
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();
		
		return s_GraphicsGlobals->Hue.Red;
	}

	void Graphics::SetRedHue(const float p_RedHue)
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue.Red = p_RedHue;
	}

	float Graphics::GetGreenHue() const
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		return s_GraphicsGlobals->Hue.Green;
	}

	void Graphics::SetGreenHue(const float p_GreenHue)
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue.Green = p_GreenHue;
	}

	float Graphics::GetBlueHue() const
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		return s_GraphicsGlobals->Hue.Blue;
	}

	void Graphics::SetBlueHue(const float p_BlueHue)
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue.Blue = p_BlueHue;
	}

	Blam::Math::RealColorRGB Graphics::GetHue() const
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		return s_GraphicsGlobals->Hue;
	}

	void Graphics::SetHue(const Blam::Math::RealColorRGB &p_Hue)
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue = p_Hue;
	}

	float Graphics::GetSaturation() const
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		return s_GraphicsGlobals->Saturation;
	}

	void Graphics::SetSaturation(const float p_Saturation)
	{
		auto *s_GraphicsGlobals = GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Saturation = p_Saturation;
	}

	float Graphics::GetBloomIntensity() const
	{
		auto *s_BloomGlobals = GetBloomGlobals();

		return s_BloomGlobals->Intensity;
	};

	void Graphics::SetBloomIntensity(const float p_Intensity)
	{
		auto *s_BloomGlobals = GetBloomGlobals();

		s_BloomGlobals->Enabled = true;
		s_BloomGlobals->Intensity = p_Intensity;
	}

	float Graphics::GetDepthOfFieldIntensity() const
	{
		auto *s_DepthOfFieldGlobals = GetDepthOfFieldGlobals();

		return s_DepthOfFieldGlobals->Intensity;
	}

	void Graphics::SetDepthOfFieldIntensity(const float p_Intensity)
	{
		auto *s_DepthOfFieldGlobals = GetDepthOfFieldGlobals();

		s_DepthOfFieldGlobals->Enabled = true;
		s_DepthOfFieldGlobals->Intensity = p_Intensity;
	}

	bool Graphics::IsLetterboxEnabled() const
	{
		auto *s_CinematicGlobals = GetCinematicGlobals();

		return s_CinematicGlobals->LetterboxEnabled != 0;
	}

	void Graphics::SetLetterboxEnabled(const bool p_Enabled)
	{
		auto *s_CinematicGlobals = GetCinematicGlobals();

		s_CinematicGlobals->LetterboxEnabled = p_Enabled;
	}
}