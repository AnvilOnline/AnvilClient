#pragma once
#include "Initializable.hpp"
#include "IdCache.hpp"
#include <cstdint>
#include <memory>

namespace AnvilCommon
{
	class Engine :
		public Initializable
	{
	public:
		virtual uint8_t* ExecutableBase() = 0;
		virtual size_t ExecutableSize() = 0;
		virtual std::shared_ptr<IdCache> GetStringCache() = 0;
	};
}