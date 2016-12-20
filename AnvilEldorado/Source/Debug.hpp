#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Module.hpp"

namespace AnvilEldorado
{
	class Debug final : public Module, public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Debug>
	{
	public:
		Command *VarMemcpySrc;
		Command *VarMemcpyDst;
		Command *VarMemsetDst;

		Debug();

		bool Init() override;
	};
}