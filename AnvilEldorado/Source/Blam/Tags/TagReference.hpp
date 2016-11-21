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

		TagReference(const Tag &p_GroupTag, const int32_t p_TagIndex)
			: GroupTag(p_GroupTag), TagIndex(p_TagIndex)
		{
		}

		TagReference()
			: TagReference(Tag::Null, -1)
		{
		}

		bool operator==(const TagReference &p_Other) const
		{
			return (GroupTag == p_Other.GroupTag)
				&& (TagIndex == p_Other.TagIndex);
		}

		bool operator!=(const TagReference &p_Other) const
		{
			return !(*this == p_Other);
		}

		explicit operator bool() const
		{
			return (GroupTag != Tag::Null)
				&& (TagIndex >= 0);
		}
	};
}
