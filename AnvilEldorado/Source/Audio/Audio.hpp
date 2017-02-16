#pragma once
#include <Interfaces\IInitializable.hpp>

namespace AnvilEldorado::Audio
{
	class Audio : 
		public AnvilCommon::IInitializable
	{
	public:
		bool Init() override;
	};
}