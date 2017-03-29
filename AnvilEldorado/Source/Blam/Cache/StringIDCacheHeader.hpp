#pragma once
#include <cstdint>

namespace Blam::Cache
{
	struct StringIDCacheHeader
	{
		int32_t Count;
		int32_t Size;
	};
}