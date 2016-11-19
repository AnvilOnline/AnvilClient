#include "Tag.hpp"

namespace Blam::Tags
{
	const Tag Tag::Null = 0xFFFFFFFF;

	Tag::Tag()
		: Tag(Tag::Null)
	{
	}

	Tag::Tag(const uint32_t p_Value)
		: Value(p_Value)
	{
	}

	Tag::Tag(const Tag &p_Other)
		: Tag(p_Other.Value)
	{
	}

	bool Tag::operator==(const Tag &p_Other) const
	{
		return Value == p_Other.Value;
	}

	bool Tag::operator!=(const Tag &p_Other) const
	{
		return !(*this == p_Other);
	}

	Tag::operator uint32_t() const
	{
		return Value;
	}

	Tag::operator bool() const
	{
		return *this != Null;
	}
}