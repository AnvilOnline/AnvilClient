#pragma once
#include <cstdint>

namespace Blam::Tags
{
	template <typename Element>
	struct TagData
	{
		int32_t Size;
		long : 32;
		long : 32;
		Element *Elements;
		long : 32;

		TagData(const int32_t p_Size, Element *const p_Elements)
			: Size(p_Size), Elements(p_Elements)
		{
		}

		TagData()
			: TagData(0, nullptr)
		{
		}

		Element *operator->() const
		{
			return Elements;
		}

		inline Element *begin() const
		{
			return Elements;
		}

		inline Element *end() const
		{
			return &Elements[Count];
		}

		Element &operator[](const size_t p_Index) const
		{
			return Elements[p_Index];
		}

		explicit operator bool() const
		{
			return (Size > 0)
				&& (Elements != nullptr);
		}
	};
	static_assert(sizeof(TagData<int32_t>) == 0x14, "Blam::Tags::TagData<>");
}