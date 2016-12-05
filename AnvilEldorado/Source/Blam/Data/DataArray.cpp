#include "DataArray.hpp"

namespace Blam::Data
{
	DataArrayBase::DataArrayBase()
		: Allocator(nullptr), Data(nullptr)
	{
	}

	DatumBase *DataArrayBase::Get(const DatumIndex &p_Index) const
	{
		return reinterpret_cast<DatumBase *(*)(const DataArrayBase *, DatumIndex)>(0x55B6D0)(this, p_Index);
	}

	DatumBase *DataArrayBase::GetAddress(const DatumIndex &p_Index) const
	{
		return reinterpret_cast<DatumBase *>(static_cast<uint8_t *>(Data) + p_Index.Index * DatumSize);
	}

	DatumBase *DataIteratorBase::Next()
	{
		return reinterpret_cast<DatumBase *(*)(DataIteratorBase *)>(0x55AE30)(this);
	}
}