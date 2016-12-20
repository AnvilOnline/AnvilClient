#include "Graphics.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	bool Engine::Init()
	{
		// TODO: Finish transitioning modules and patches into singleton classes

		return ApplyPatches_Core()
			&& ApplyPatches_Audio()
			&& ApplyPatches_Input()
			&& ApplyPatches_Network()
			&& ApplyPatches_Content()
			&& ApplyPatches_Game()
			&& ApplyPatches_Forge()
			&& ApplyPatches_Scoreboard()
			&& ApplyPatches_UserInterface()
			&& ApplyPatches_VirtualKeyboard()
			&& Graphics::Instance()->Init()
			&& Camera::Instance()->Init()
			&& Player::Instance()->Init();
	}

	bool Engine::OnTagsLoaded()
	{
		return Player::Instance()->OnTagsLoaded();
	}
}