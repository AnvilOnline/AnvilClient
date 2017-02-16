#pragma once
#include <memory>
#include <Interfaces/IEngineInitializable.hpp>

namespace Anvil::Client
{
	class AnvilClient : 
		public AnvilCommon::IInitializable
	{
	private:
		std::shared_ptr<AnvilCommon::IEngineInitializable> m_Engine;

	public:
		static std::shared_ptr<AnvilClient> Instance();

		virtual bool Init() override;
	};
}