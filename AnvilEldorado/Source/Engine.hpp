#pragma once
#include <Interfaces/IInitializable.hpp>

void *GetModuleBase();

namespace AnvilEldorado
{
	class Engine : public AnvilCommon::IInitializable
	{
	public:
		virtual bool Init() override;
	};
}