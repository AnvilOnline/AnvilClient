#pragma once
#include <cstdint>
#include <string>
#include "Blam\Text\StringID.hpp"

namespace Blam::Cache
{
	struct StringIDCacheHeader
	{
		int32_t Count;
		int32_t Size;
	};
}