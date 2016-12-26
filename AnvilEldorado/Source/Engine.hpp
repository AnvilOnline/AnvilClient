#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Engine final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Engine>
	{
	public:
		bool Init() override;

		bool UnprotectMemory();
		bool ApplyPatches();

		bool OnTagsLoaded();

		bool HasMainMenuShown() const;
		void SetMainMenuShown(const bool p_Shown = true);

	private:
		bool m_MainMenuShown = false;
	};
}