#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Engine final : public AnvilCommon::IInitializable, public AnvilCommon::Utils::Singleton<Engine>
	{
	private:
		bool ApplyPatches_Core();
		bool ApplyPatches_Camera();
		bool ApplyPatches_Graphics();
		bool ApplyPatches_Input();
		bool ApplyPatches_Network();
		bool ApplyPatches_Content();
		bool ApplyPatches_UserInterface();
		bool ApplyPatches_Armor();
		bool ApplyPatches_Player();
		bool ApplyPatches_Forge();
		bool ApplyPatches_GameRules();
		bool ApplyPatches_Scoreboard();
		bool ApplyPatches_VirtualKeyboard();
	
		bool OnTagsLoaded_Armor();

	public:
		inline bool ApplyPatches()
		{
			return ApplyPatches_Core()
				&& ApplyPatches_Camera()
				&& ApplyPatches_Graphics()
				&& ApplyPatches_Input()
				&& ApplyPatches_Network()
				&& ApplyPatches_Content()
				&& ApplyPatches_UserInterface()
				&& ApplyPatches_Armor()
				&& ApplyPatches_Player()
				&& ApplyPatches_Forge()
				&& ApplyPatches_GameRules()
				&& ApplyPatches_Scoreboard()
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