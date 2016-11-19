#include "TagReference.hpp"

namespace Blam::Tags
{
	TagReference::TagReference()
		: TagReference(Tag::Null, -1)
	{
	}

	TagReference::TagReference(const Tag &p_GroupTag, const int32_t p_TagIndex)
		: GroupTag(p_GroupTag), TagIndex(p_TagIndex)
	{
	}

	bool TagReference::operator==(const TagReference &p_Other) const
	{
		return GroupTag == p_Other.GroupTag
			&& TagIndex == p_Other.TagIndex;
	}

	bool TagReference::operator!=(const TagReference &p_Other) const
	{
		return !(*this == p_Other);
	}

	TagReference::operator bool() const
	{
		return GroupTag != (Tag)-1
			&& TagIndex != -1;
	}
}