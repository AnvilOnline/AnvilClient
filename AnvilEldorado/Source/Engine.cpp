#include "Graphics.hpp"
#include "Camera.hpp"
#include "UserInterface.hpp"
#include "Game.hpp"
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
			&& ApplyPatches_Forge()
			&& ApplyPatches_Scoreboard()
			&& ApplyPatches_VirtualKeyboard()
			&& Graphics::Instance()->Init()
			&& Camera::Instance()->Init()
			&& UserInterface::Instance()->Init()
			&& Game::Instance()->Init()
			&& Player::Instance()->Init();
	}

	bool Engine::OnTagsLoaded()
	{
		return Player::Instance()->OnTagsLoaded();
	}

	bool Engine::HasMainMenuShown() const
	{
		return m_MainMenuShown;
	}

	void Engine::SetMainMenuShown(const bool p_Shown)
	{
		m_MainMenuShown = p_Shown;
	}
}