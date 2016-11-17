#pragma once
#include <Misc/IInitializable.hpp>
#include <memory>

namespace Anvil::Client
{
	class AnvilClient : IInitializable
	{
	public:
		static std::shared_ptr<AnvilClient> GetInstance();

		virtual bool Init() override;
	};
}