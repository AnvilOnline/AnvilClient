#pragma once
#include <cstdint>

namespace Blam::Data
{
	struct DatumIndex
	{
		static const DatumIndex Null;

		union
		{
			uint32_t Value;
			struct
			{
				uint16_t Salt;
				uint16_t Index;
			};
		};

		DatumIndex();
		DatumIndex(const uint32_t p_Value);
		DatumIndex(const uint16_t p_Salt, const uint16_t p_Index);

		bool operator==(const DatumIndex &other) const;
		bool operator!=(const DatumIndex &other) const;

		explicit operator uint32_t() const;

		explicit operator bool() const;
	};
	static_assert(sizeof(DatumIndex) == 0x4, "Blam::Data::DatumIndex");
}