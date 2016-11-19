#pragma once
#include <cstdint>
#include "Tag.hpp"

namespace Blam::Tags
{
	struct TagReference
	{
		Tag GroupTag;
		int32_t : 32;
		int32_t : 32;
		int32_t TagIndex;

		TagReference();
		TagReference(const Tag &p_GroupTag, const int32_t p_TagIndex);

		bool operator==(const TagReference &p_Other) const;
		bool operator!=(const TagReference &p_Other) const;

		explicit operator bool() const;
	};
}
