#pragma once
#include <Interfaces\Initializable.hpp>

namespace AnvilEldorado
{
	class Engine : 
		public AnvilCommon::Initializable
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