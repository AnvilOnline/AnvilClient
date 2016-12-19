#pragma once
#include "Utils\Singleton.hpp"
#include "ModuleBase.hpp"

namespace AnvilEldorado::Modules
{
	class ModuleDebug : public ModuleBase, public AnvilCommon::Singleton<ModuleDebug>
	{
	public:
		Command *VarMemcpySrc;
		Command *VarMemcpyDst;
		Command *VarMemsetDst;

		ModuleDebug();
	};
}