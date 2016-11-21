#pragma once
#include <cstdint>

namespace Blam::Tags
{
	template <typename Element>
	struct TagBlock
	{
		int32_t Count;
		Element *Elements;
		unsigned : 32;

		TagBlock(const int32_t p_Count, Element *const p_Elements)
			: Count(p_Count), Elements(p_Elements)
		{
		}

		TagBlock()
			: TagBlock(0, nullptr)
		{
		}

		Element *operator->() const
		{
			return Elements;
		}

		Element &operator[](const size_t p_Index) const
		{
			return Elements[p_Index];
		}

		explicit operator bool() const
		{
			return (Count > 0)
				&& (Elements != nullptr);
		}
	};
	static_assert(sizeof(TagBlock<void>) == 0xC, "Blam::Tags::TagBlock<>");
}