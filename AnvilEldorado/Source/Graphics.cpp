#include <sstream>
#include "Globals.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"

namespace AnvilEldorado
{
	const auto ApplyResolutionChange = reinterpret_cast<void(__thiscall *)()>(0xA226D0);

	void ResolutionChangeHook()
	{
		// Update the ingame UI's resolution
		ApplyResolutionChange();

		//
		// TODO: Apply new resolution to any subsystems...
		//
	}

	double AspectRatioHook()
	{
		auto *s_GameResolution = reinterpret_cast<int32_t *>(0x19106C0);

		return ((double)s_GameResolution[0] / (double)s_GameResolution[1]);
	}

	bool VariableSaturationUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
	{
		auto s_Saturation = Graphics::Instance()->VarSaturation->ValueFloat;

		auto *s_GraphicsGlobals = Graphics::GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Saturation = s_Saturation;

		std::stringstream ss;
		ss << "Set saturation to " << s_Saturation;

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableRedHueUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
	{
		auto s_RedHue = Graphics::Instance()->VarRedHue->ValueFloat;

		auto *s_GraphicsGlobals = Graphics::GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue.Red = s_RedHue;

		std::stringstream ss;
		ss << "Set red hue to " << s_RedHue;

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableGreenHueUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
	{
		auto s_GreenHue = Graphics::Instance()->VarGreenHue->ValueFloat;

		auto *s_GraphicsGlobals = Graphics::GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue.Green = s_GreenHue;

		std::stringstream ss;
		ss << "Set green hue to " << s_GreenHue;

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableBlueHueUpdate(const std::vector<std::string> &Arguments, std::string &p_ReturnInfo)
	{
		auto s_BlueHue = Graphics::Instance()->VarBlueHue->ValueFloat;

		auto *s_GraphicsGlobals = Graphics::GetGraphicsGlobals();

		s_GraphicsGlobals->Override = true;
		s_GraphicsGlobals->Hue.Blue = s_BlueHue;

		std::stringstream ss;
		ss << "Set blue hue to " << s_BlueHue;

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableBloomUpdate(const std::vector<std::string> &Arguments, std::string &p_ReturnInfo)
	{
		auto s_BloomIntensity = Graphics::Instance()->VarBloom->ValueFloat;

		auto *s_BloomGlobals = Graphics::GetBloomGlobals();

		s_BloomGlobals->Enabled = true;
		s_BloomGlobals->Intensity = s_BloomIntensity;

		std::stringstream ss;
		ss << "Set bloom intensity to " << s_BloomIntensity;

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableDepthOfFieldUpdate(const std::vector<std::string> &Arguments, std::string &p_ReturnInfo)
	{
		auto s_DepthOfFieldIntensity = Graphics::Instance()->VarDepthOfField->ValueFloat;

		auto *s_DepthOfFieldGlobals = Graphics::GetDepthOfFieldGlobals();

		s_DepthOfFieldGlobals->Enabled = true;
		s_DepthOfFieldGlobals->Intensity = s_DepthOfFieldIntensity;

		std::stringstream ss;
		ss << "Set depth of field intensity to " << s_DepthOfFieldIntensity;

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableLetterboxUpdate(const std::vector<std::string> &Arguments, std::string &p_ReturnInfo)
	{
		auto s_LetterboxEnabled = Graphics::Instance()->VarLetterbox->ValueInt;

		auto *s_CinematicGlobals = Graphics::GetCinematicGlobals();

		s_CinematicGlobals->LetterboxEnabled = s_LetterboxEnabled;

		std::stringstream ss;
		ss << (s_LetterboxEnabled ? "Enabled" : "Disabled") << " letterbox";

		p_ReturnInfo = ss.str();

		return true;
	}

	bool VariableUIScalingUpdate(const std::vector<std::string> &Arguments, std::string &p_ReturnInfo)
	{
		if (Graphics::Instance()->VarUIScaling->ValueInt == 0)
			p_ReturnInfo = "The changes will apply on game restart";
		else if (Engine::Instance()->HasMainMenuShown()) //If the user comes to their senses and turns UI scaling back on then apply the change. Running before the main menu has shown may result in a crash.
			/* TODO: Patches::Ui::ApplyUIResolution(); */
			return true;

		return true;
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

	Graphics::Graphics()
		: Module("Graphics")
	{
		VarSaturation = AddVariableFloat("Saturation", "saturation", "The saturation", CommandFlags::Archived, 1.0f, VariableSaturationUpdate);
		VarSaturation->ValueFloatMin = -10.0f;
		VarSaturation->ValueFloatMax = 10.0f;

		VarRedHue = AddVariableFloat("RedHue", "red_hue", "The red hue", CommandFlags::None, 1.0f, VariableRedHueUpdate);
		VarRedHue->ValueFloatMin = 0.0f;
		VarRedHue->ValueFloatMax = 1.0f;

		VarGreenHue = AddVariableFloat("GreenHue", "green_hue", "The green hue", CommandFlags::None, 1.0f, VariableGreenHueUpdate);
		VarGreenHue->ValueFloatMin = 0.0f;
		VarGreenHue->ValueFloatMax = 1.0f;

		VarBlueHue = AddVariableFloat("BlueHue", "blue_hue", "The blue hue", CommandFlags::None, 1.0f, VariableBlueHueUpdate);
		VarBlueHue->ValueFloatMin = 0.0f;
		VarBlueHue->ValueFloatMax = 1.0f;

		// TODO: consider breaking some of these out into a separate cinematics module or possibly moving dof to camera

		VarBloom = AddVariableFloat("Bloom", "bloom", "The atmosphere bloom", CommandFlags::Archived, 0.0f, VariableBloomUpdate);
		VarBloom->ValueFloatMin = 0.0f;
		VarBloom->ValueFloatMax = 5.0f;

		VarDepthOfField = AddVariableFloat("DepthOfField", "dof", "The camera's depth of field", CommandFlags::None, 0.0f, VariableDepthOfFieldUpdate);
		VarDepthOfField->ValueFloatMin = 0.0f;
		VarDepthOfField->ValueFloatMax = 1.0f;

		VarLetterbox = AddVariableInt("Letterbox", "letterbox", "A cinematic letterbox.", CommandFlags::None, 0, VariableLetterboxUpdate);
		VarLetterbox->ValueIntMin = 0;
		VarLetterbox->ValueIntMax = 1;

		VarUIScaling = AddVariableInt("UIScaling", "uiscaling", "Enables proper UI scaling to match your monitor's resolution.", CommandFlags::Archived, 1, VariableUIScalingUpdate);
		VarUIScaling->ValueIntMin = 0;
		VarUIScaling->ValueIntMax = 1;
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
}