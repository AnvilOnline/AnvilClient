#pragma once
#include <Interfaces/Initializable.hpp>
#include <memory>

namespace AnvilEldorado::Game
{
	class Hooks;

	class GameImpl :
		public AnvilCommon::Initializable
	{
	protected:
		std::shared_ptr<Hooks> m_Hooks;

	public:
		virtual bool Init();
	};
}