#pragma once
#include "Initializable.hpp"
#include <cstdint>

namespace AnvilCommon
{
	class Engine :
		public Initializable
	{
	public:
		virtual uint8_t* ExecutableBase() = 0;
		virtual size_t ExecutableSize() = 0;
	};
}