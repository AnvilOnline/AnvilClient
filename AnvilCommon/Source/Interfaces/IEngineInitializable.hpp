#pragma once
#include "IInitializable.hpp"
#include <cstdint>

namespace AnvilCommon
{
	class IEngineInitializable :
		public IInitializable
	{
	public:
		virtual uint8_t* ExecutableBase() = 0;
		virtual size_t ExecutableSize() = 0;
	};
}