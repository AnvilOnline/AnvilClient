#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Engine final : public AnvilCommon::IInitializable, public AnvilCommon::Utils::Singleton<Engine>
	{
	private:
		bool ApplyPatches_Core();
		bool ApplyPatches_Audio();
		bool ApplyPatches_Graphics();
		bool ApplyPatches_Input();
		bool ApplyPatches_Network();
		bool ApplyPatches_Content();
		bool ApplyPatches_Armor();
		bool ApplyPatches_Player();
		bool ApplyPatches_Game();
		bool ApplyPatches_Forge();
		bool ApplyPatches_Scoreboard();
		bool ApplyPatches_UserInterface();
		bool ApplyPatches_VirtualKeyboard();
	
		bool OnTagsLoaded_Armor();

	public:
		inline bool ApplyPatches()
		{
			return ApplyPatches_Core()
				&& ApplyPatches_Audio()
				&& ApplyPatches_Graphics()
				&& ApplyPatches_Input()
				&& ApplyPatches_Network()
				&& ApplyPatches_Content()
				&& ApplyPatches_Armor()
				&& ApplyPatches_Player()
				&& ApplyPatches_Forge()
				&& ApplyPatches_Game()
				&& ApplyPatches_Scoreboard()
				&& ApplyPatches_UserInterface()
				&& ApplyPatches_VirtualKeyboard();
		}

		inline bool OnTagsLoaded()
		{
			return OnTagsLoaded_Armor();
		}

		inline bool Init() override
		{
			return ApplyPatches();
		}
	};
}