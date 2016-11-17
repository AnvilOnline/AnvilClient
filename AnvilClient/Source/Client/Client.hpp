#pragma once
#include <Interfaces/IInitializable.hpp>
#include <memory>

namespace Anvil::Client
{
	class AnvilClient : AnvilCommon::IInitializable
	{
	private:
		std::shared_ptr<AnvilCommon::IInitializable> m_Engine;

	public:
		static std::shared_ptr<AnvilClient> GetInstance();

		virtual bool Init() override;
	};
}