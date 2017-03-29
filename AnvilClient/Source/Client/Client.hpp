#pragma once
#include <memory>
#include <Interfaces\Initializable.hpp>


namespace Anvil::Client
{
	class AnvilClient : 
		public AnvilCommon::Initializable
	{
	private:
		std::shared_ptr<AnvilCommon::Initializable> m_Engine;

	public:
		static std::shared_ptr<AnvilClient> GetInstance();
		virtual bool Init() override;
	};
}