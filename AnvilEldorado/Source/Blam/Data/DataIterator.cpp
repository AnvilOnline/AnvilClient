#include "DataIterator.hpp"

namespace Blam::Data
{
	DataIteratorBase::DataIteratorBase(const DataArrayBase *data)
		: Array(data), CurrentDatumIndex(DatumIndex::Null), CurrentIndex(-1)
	{
	}

	DatumBase *DataIteratorBase::Next()
	{
		return reinterpret_cast<DatumBase *(*)(DataIteratorBase *)>(0x55AE30)(this);
	}
}