#pragma once
#include <Misc/IInit.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Functions
		{
			class SDKFunctions : public IInit
			{
			public:
				bool Init();
			};
		}
	}
}