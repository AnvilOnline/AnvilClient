#pragma once
#include <memory>
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace Anvil::Client
{
	class AnvilClient : AnvilCommon::IInitializable, public AnvilCommon::Singleton<AnvilClient>
	{
	private:
		std::shared_ptr<AnvilCommon::IInitializable> m_Engine;

	public:
		virtual bool Init() override;
	};
}