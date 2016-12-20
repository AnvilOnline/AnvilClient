#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Engine final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Engine>
	{
	private:
		bool ApplyPatches_Core();
		bool ApplyPatches_Audio();
		bool ApplyPatches_Input();
		bool ApplyPatches_Network();
		bool ApplyPatches_Content();
		bool ApplyPatches_Game();
		bool ApplyPatches_Forge();
		bool ApplyPatches_Scoreboard();
		bool ApplyPatches_UserInterface();
		bool ApplyPatches_VirtualKeyboard();
	
		bool m_MainMenuShown = false;

	public:
		bool Init() override;

		bool OnTagsLoaded();

		inline bool HasMainMenuShown() const
		{
			return m_MainMenuShown;
		}

		inline void SetMainMenuShown(const bool p_Shown = true)
		{
			m_MainMenuShown = p_Shown;
		}
	};
}