#pragma once
#include <cstdint>

namespace Blam::Tags
{
	struct Tag
	{
		static const Tag Null;

		uint32_t Value;

		Tag(const uint32_t p_Value)
			: Value(p_Value)
		{
		}

		Tag(const Tag &p_Other)
			: Tag(p_Other.Value)
		{
		}

		Tag()
			: Tag(0xFFFFFFFF)
		{
		}

		bool operator==(const Tag &p_Other) const
		{
			return Value == p_Other.Value;
		}

		bool operator!=(const Tag &p_Other) const
		{
			return !(*this == p_Other);
		}

		explicit operator uint32_t() const
		{
			return Value;
		}

		explicit operator bool() const
		{
			return Value != Null.Value;
		}
	};

	const Tag Tag::Null = 0xFFFFFFFF;
}