#include "BlamData.hpp"

namespace Blam
{
	Data::DatumBase* DataArrayBase::Get(Data::DatumIndex index) const
	{
		typedef Data::DatumBase*(*DataArrayGetPtr)(const DataArrayBase *data, Data::DatumIndex index);
		auto DataArrayGet = reinterpret_cast<DataArrayGetPtr>(0x55B6D0);
		return DataArrayGet(this, index);
	}

	Data::DatumBase* DataIteratorBase::Next()
	{
		typedef Data::DatumBase*(*DataIteratorNextPtr)(DataIteratorBase *iterator);
		auto DataIteratorNext = reinterpret_cast<DataIteratorNextPtr>(0x55AE30);
		return DataIteratorNext(this);
	}

	int CalculateDatumArraySize(int datumCount, int datumSize, int alignmentBits)
	{
		return reinterpret_cast<int(__cdecl*)(int, int, int)>(0x55AAB0)(datumCount, datumSize, alignmentBits);
	}
}