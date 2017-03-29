#pragma once
#include <Interfaces\Initializable.hpp>

namespace AnvilEldorado
{
	class Audio : 
		public AnvilCommon::Initializable
	{
	public:
		virtual bool Init();
	};
}