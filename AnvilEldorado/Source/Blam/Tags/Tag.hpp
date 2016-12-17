#pragma once
#include <cstdint>

namespace Blam::Tags
{
	struct Tag
	{
		static const Tag Null;

		uint32_t Value;
		
		Tag();
		Tag(const uint32_t p_Value);
		Tag(const Tag &p_Other);

		bool operator==(const Tag &p_Other) const;
		bool operator!=(const Tag &p_Other) const;

		explicit operator uint32_t() const;

		explicit operator bool() const;
	};
}