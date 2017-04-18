#pragma once
#include <Interfaces\Initializable.hpp>
#include <memory>

namespace AnvilEldorado::Game::Audio
{
	class AudioImpl :
		public AnvilCommon::Initializable
	{
	private:
		std::shared_ptr<AnvilCommon::Initializable> m_Hooks;
	public:
		virtual bool Init();
	};
}