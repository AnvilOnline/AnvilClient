#pragma once
#include <cstdint>
#include "DatumIndex.hpp"
#include "DatumBase.hpp"
#include "DataArrayBase.hpp"

namespace Blam::Data
{
	struct DataIteratorBase
	{
		const DataArrayBase *Array;
		DatumIndex CurrentDatumIndex;
		int32_t CurrentIndex;

		explicit DataIteratorBase(const DataArrayBase *data);

		DatumBase *Next();
	};
	static_assert(sizeof(DataIteratorBase) == 0xC, "Blam::Data::DataIteratorBase");
}