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


		TDatum* Get(DatumIndex p_Index) const
		{
			return static_cast<TDatum *>(DataArrayBase::Get(p_Index));
		}

		TDatum &operator[](DatumIndex p_Index) const
		{
			return *static_cast<TDatum *>(GetAddress(index));
		}

		DataIterator<TDatum> begin()
		{
			DataIterator<TDatum> result(this);
			result.Next();
			return result;
		}

		DataIterator<TDatum> end()
		{
			DataIterator<TDatum> result(this);
			result.CurrentIndex = MaxCount;
			return result;
		}

		ConstDataIterator<TDatum> begin() const
		{
			ConstDataIterator<TDatum> result(this);
			result.Next();
			return result;
		}

		ConstDataIterator<TDatum> end() const
		{
			ConstDataIterator<TDatum> result(this);
			result.CurrentIndex = MaxCount;
			return result;
		}
	};
	static_assert(sizeof(DataArray<DatumBase>) == sizeof(DataArrayBase), "Blam::Data::DataArray<TDatum>");
}