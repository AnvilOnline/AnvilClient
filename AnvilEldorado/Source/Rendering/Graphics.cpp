#include "Graphics.hpp"

#include <Globals.hpp>

#include "Globals/Bloom.hpp"
#include "Globals/Cinematic.hpp"
#include "Globals/DepthOfField.h"
#include "Globals/Graphics.hpp"

using namespace AnvilEldorado::Rendering;

bool Graphics::Init()
{
	
	return true;

	//using AnvilCommon::Utils::HookFlags;
	//using AnvilCommon::Utils::Hook;
	//using AnvilCommon::Utils::Patch;

	//return true;
	//// Runs when the game's resolution is changed
	//// TODO: Hook 00A226D0
	//return Hook(0x621303, ResolutionChangeHook, HookFlags::IsCall).Apply()
	//	// Fix aspect ratio not matching resolution
	//	&& Hook(0x6648C9, AspectRatioHook, HookFlags::IsCall).Apply()
	//	&& Hook(0x216487, AspectRatioHook, HookFlags::IsCall).Apply()
	//	// Disable converting the game's resolution to 16:9
	//	// TODO: See if sub_42E350 is what we need for all 4 patches below
	//	&& Patch::NopFill(0x62217D, 2)
	//	&& Patch::NopFill(0x622183, 6)
	//	// Allow the user to select any resolution that Windows supports in the settings screen
	//	&& Patch::NopFill(0x10BF1B, 2)
	//	&& Patch::NopFill(0x10BF21, 6);
}


// TODO: Move this
void Graphics::OnResolutionChange()
{
	// TODO: Move this to a global way to grab this
	const auto ApplyResolutionChange = reinterpret_cast<void(__thiscall *)()>(0xA226D0);

	ApplyResolutionChange();

	// 
	//UserInterface::Instance()->ApplyResolution();
}

// TODO: Hook 00A1FA30 - ComputeAspectRatio
double AspectRatioHook()
{
	auto *s_GameResolution = reinterpret_cast<int32_t *>(0x19106C0);

	return ((double)s_GameResolution[0] / (double)s_GameResolution[1]);
}

Globals::Bloom* Graphics::GetBloomGlobals()
{
	return AnvilCommon::GetThreadStorage<Globals::Bloom>(0x104);
}

Globals::Cinematic* Graphics::GetCinematicGlobals()
{
	return AnvilCommon::GetThreadStorage<Globals::Cinematic>(0x324);
}

Globals::DepthOfField* Graphics::GetDepthOfFieldGlobals()
{
	return AnvilCommon::GetThreadStorage<Globals::DepthOfField>(0x3DC);
}

Globals::Graphics* Graphics::GetGraphicsGlobals()
{
	return AnvilCommon::GetThreadStorage<Globals::Graphics>(0x3C0);
}

float Graphics::GetRedHue()
{
	return GetGraphicsGlobals()->Hue.Red;
}

void Graphics::SetRedHue(float p_RedHue)
{
	auto s_GraphicsGlobals = GetGraphicsGlobals();

	s_GraphicsGlobals->Override = true;
	s_GraphicsGlobals->Hue.Red = p_RedHue;
}

float Graphics::GetGreenHue()
{
	return GetGraphicsGlobals()->Hue.Green;
}

void Graphics::SetGreenHue(float p_GreenHue)
{
	auto s_GraphicsGlobals = GetGraphicsGlobals();

	s_GraphicsGlobals->Override = true;
	s_GraphicsGlobals->Hue.Green = p_GreenHue;
}

float Graphics::GetBlueHue()
{
	return GetGraphicsGlobals()->Hue.Blue;
}

void Graphics::SetBlueHue(float p_BlueHue)
{
	auto s_GraphicsGlobals = GetGraphicsGlobals();

	s_GraphicsGlobals->Override = true;
	s_GraphicsGlobals->Hue.Blue = p_BlueHue;
}

Blam::Math::RealColorRGB Graphics::GetHue()
{
	return GetGraphicsGlobals()->Hue;
}

void Graphics::SetHue(Blam::Math::RealColorRGB &p_Hue)
{
	auto s_GraphicsGlobals = GetGraphicsGlobals();

	s_GraphicsGlobals->Override = true;
	s_GraphicsGlobals->Hue = p_Hue;
}

float Graphics::GetSaturation()
{
	return GetGraphicsGlobals()->Saturation;
}

void Graphics::SetSaturation(const float p_Saturation)
{
	auto s_GraphicsGlobals = GetGraphicsGlobals();

	s_GraphicsGlobals->Override = true;
	s_GraphicsGlobals->Saturation = p_Saturation;
}

float Graphics::GetBloomIntensity()
{
	return GetBloomGlobals()->Intensity;
};

void Graphics::SetBloomIntensity(float p_Intensity)
{
	auto s_BloomGlobals = GetBloomGlobals();

	s_BloomGlobals->Enabled = true;
	s_BloomGlobals->Intensity = p_Intensity;
}

float Graphics::GetDepthOfFieldIntensity()
{
	return GetDepthOfFieldGlobals()->Intensity;
}

void Graphics::SetDepthOfFieldIntensity(float p_Intensity)
{
	auto s_DepthOfFieldGlobals = GetDepthOfFieldGlobals();

	s_DepthOfFieldGlobals->Enabled = true;
	s_DepthOfFieldGlobals->Intensity = p_Intensity;
}

bool Graphics::IsLetterboxEnabled()
{
	return GetCinematicGlobals()->LetterboxEnabled != 0;
}

void Graphics::SetLetterboxEnabled(bool p_Enabled)
{
	GetCinematicGlobals()->LetterboxEnabled = p_Enabled;
}