#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Interfaces\IInitializable.hpp"

#include "Utils\Singleton.hpp"

#include "Blam\Text\StringID.hpp"

namespace AnvilEldorado
{
	class UserInterface final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<UserInterface>
	{
	public:
		bool Init() override;

		typedef std::function<void(HWND window)> CreateWindowCallback;
		HWND CreateGameWindow();

		bool ShouldScaleInterface() const;
		void SetScaleInterface(const bool p_ScaleInterface = true);

		void ApplyResolution();

		void *ShowDialog(const Blam::Text::StringID &p_DialogID, const int32_t p_Arg1 = 0, const int32_t p_Flags = 4, const Blam::Text::StringID &p_ParentID = Blam::Text::StringID::Null);

	private:
		std::vector<CreateWindowCallback> m_CreateWindowCallbacks;

		bool m_ScaleInterface = true;

		int m_HudResolutionWidth = 0;
		int m_HudResolutionHeight = 0;
		float m_HudResolutionScaleX = 0;
		float m_HudResolutionScaleY = 0;
		float m_HudMotionSensorOffsetX = 0;
		float m_HudBottomVisorOffsetY = 0;
		bool m_FirstResolutionChange = true;
	};
}