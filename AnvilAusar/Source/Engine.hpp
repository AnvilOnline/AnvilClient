#pragma once
#include <Interfaces/IInitializable.hpp>

namespace AnvilAusar
{
	class Engine : public AnvilCommon::IInitializable
	{
	public:
		virtual bool Init() override;
	};
}