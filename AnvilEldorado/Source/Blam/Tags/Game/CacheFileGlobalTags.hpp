#pragma once
#include <cstdint>
#include "Blam\Tags\TagBlock.hpp"
#include "Blam\Tags\TagReference.hpp"

namespace Blam::Tags::Game
{
	struct CacheFileGlobalTags
	{
		TagBlock<TagReference> GlobalTags;
		int32_t : 32;
	};
}