#pragma once
#include <Interfaces\Initializable.hpp>
#include <memory>

namespace AnvilEldorado::Game::Networking
{
	class NetworkingImpl :
		public AnvilCommon::Initializable
	{
	private:
		std::shared_ptr<AnvilCommon::Initializable> m_Hooks;

	public:
		virtual bool Init();
	};
}