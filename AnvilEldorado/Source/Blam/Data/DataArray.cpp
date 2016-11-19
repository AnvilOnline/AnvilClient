#include "DataArray.hpp"

namespace Blam::Data
{
	template<class TDatum>
	TDatum* DataArray<TDatum>::Get(DatumIndex p_Index) const
	{
		return static_cast<TDatum *>(DataArrayBase::Get(p_Index));
	}

	template<class TDatum>
	TDatum &DataArray<TDatum>::operator[](DatumIndex p_Index) const
	{
		return *static_cast<TDatum *>(GetAddress(index));
	}

	template<class TDatum>
	DataIterator<TDatum> DataArray<TDatum>::begin()
	{
		DataIterator<TDatum> result(this);
		result.Next();
		return result;
	}

	template<class TDatum>
	DataIterator<TDatum> DataArray<TDatum>::end()
	{
		DataIterator<TDatum> result(this);
		result.CurrentIndex = MaxCount;
		return result;
	}

	template<class TDatum>
	ConstDataIterator<TDatum> DataArray<TDatum>::begin() const
	{
		ConstDataIterator<TDatum> result(this);
		result.Next();
		return result;
	}

	template<class TDatum>
	ConstDataIterator<TDatum> DataArray<TDatum>::end() const
	{
		ConstDataIterator<TDatum> result(this);
		result.CurrentIndex = MaxCount;
		return result;
	}
}