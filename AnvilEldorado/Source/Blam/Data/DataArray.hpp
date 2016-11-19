#pragma once
#include <type_traits>
#include "DatumIndex.hpp"
#include "DatumBase.hpp"
#include "DataArrayBase.hpp"

namespace Blam::Data
{
	template<class TDatum> struct DataIterator;
	template<class TDatum> struct ConstDataIterator;

	template<class TDatum>
	struct DataArray : DataArrayBase
	{
		static_assert(std::is_base_of<DatumBase, TDatum>::value, "Blam::Data::DataArray<TDatum>: TDatum must inherit from Blam::Data::DatumBase.");

		TDatum* Get(DatumIndex p_Index) const;

		TDatum& operator[](DatumIndex p_Index) const;

		DataIterator<TDatum> begin();
		DataIterator<TDatum> end();

		ConstDataIterator<TDatum> begin() const;
		ConstDataIterator<TDatum> end() const;
	};
	static_assert(sizeof(DataArray<DatumBase>) == sizeof(DataArrayBase), "Blam::Data::DataArray<TDatum>");
}