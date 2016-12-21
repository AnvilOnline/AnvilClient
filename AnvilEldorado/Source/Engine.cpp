#include "Graphics.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "UserInterface.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Forge.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	bool Engine::Init()
	{
		// TODO: Finish transitioning modules and patches into singleton classes

		return ApplyPatches_Core()
			&& ApplyPatches_Audio()
			&& ApplyPatches_Network()
			&& ApplyPatches_Content()
			&& ApplyPatches_Scoreboard()
			&& ApplyPatches_VirtualKeyboard()
			&& Graphics::Instance()->Init()
			&& Input::Instance()->Init()
			&& Camera::Instance()->Init()
			&& UserInterface::Instance()->Init()
			&& Game::Instance()->Init()
			&& Player::Instance()->Init()
			&& Forge::Instance()->Init();
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