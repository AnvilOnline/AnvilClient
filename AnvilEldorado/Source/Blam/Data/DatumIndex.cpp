#include "DatumIndex.hpp"

namespace Blam::Data
{
	const DatumIndex DatumIndex::Null = { 0xFFFF, 0xFFFF };

	DatumIndex::DatumIndex()
		: DatumIndex(DatumIndex::Null.Value)
	{
	}

	DatumIndex::DatumIndex(const uint32_t p_Value)
		: Value(p_Value)
	{
	}

	DatumIndex::DatumIndex(const uint16_t p_Salt, const uint16_t p_Index)
		: Salt(p_Salt), Index(p_Index)
	{
	}

	bool DatumIndex::operator==(const DatumIndex &other) const
	{
		return (Salt == other.Salt)
			&& (Index == other.Index);
	}

	bool DatumIndex::operator!=(const DatumIndex &other) const
	{
		return !(*this == other);
	}

	DatumIndex::operator uint32_t() const
	{
		return Value;
	}

	DatumIndex::operator bool() const
	{
		return *this != Null;
	}
}