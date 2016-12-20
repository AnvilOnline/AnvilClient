#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Engine final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Engine>
	{
	public:
		bool Init() override;

		bool OnTagsLoaded();

		bool HasMainMenuShown() const;
		void SetMainMenuShown(const bool p_Shown = true);

	private:
		bool m_MainMenuShown = false;

		bool ApplyPatches_Core();
		bool ApplyPatches_Audio();
		bool ApplyPatches_Input();
		bool ApplyPatches_Network();
		bool ApplyPatches_Content();
		bool ApplyPatches_Forge();
		bool ApplyPatches_Scoreboard();
		bool ApplyPatches_VirtualKeyboard();
	};
}