#pragma once
#include <Interfaces\Initializable.hpp>
#include <cstdint>
#include <string>

namespace AnvilCommon
{
	class IdCache :
		public Initializable
	{
	public:
		virtual bool Empty() = 0;
		virtual int32_t Count() = 0;
		virtual std::string GetString(int16_t p_Id) = 0;
	};
}