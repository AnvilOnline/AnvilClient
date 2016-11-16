#pragma once
#include <Misc/IInitializable.hpp>

namespace Anvil::Client
{
	class AnvilClient : IInitializable
	{
	public:
		virtual bool Init() override;
	};
}