#pragma once
#include <Misc/IInit.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Patches
		{
			class EnginePatches : public IInit
			{
			public:
				bool Init() override;
			};
		}
	}
}