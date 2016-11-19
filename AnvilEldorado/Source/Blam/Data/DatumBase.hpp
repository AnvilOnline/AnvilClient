#pragma once
#include <cstdint>

namespace Blam::Data
{
	struct DatumBase
	{
		static const DatumBase Null;

		uint16_t Salt;

		DatumBase();
		DatumBase(const uint16_t p_Salt);

		bool operator==(const DatumBase &p_Other) const;
		bool operator!=(const DatumBase &p_Other) const;

		explicit operator uint16_t() const;

		explicit operator bool() const;
	};
	static_assert(sizeof(DatumBase) == 0x2, "Blam::Data::DatumBase");
}