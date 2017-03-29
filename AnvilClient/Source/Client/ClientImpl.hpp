#pragma once
#include <memory>
#include <Interfaces/Client.hpp>

namespace Anvil::Client
{
	class ClientImpl :
		public AnvilCommon::Client
	{
	private:
		std::shared_ptr<AnvilCommon::Engine> m_Engine;

	public:
		static std::shared_ptr<ClientImpl> GetInstance();

		virtual bool Init();
		virtual std::shared_ptr<AnvilCommon::Engine> GetEngine();
	};
}