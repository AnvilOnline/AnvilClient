#include "DataIterator.hpp"

namespace Blam::Data
{
	template <class TDatum>
	DataIterator<TDatum>::DataIterator()
		: DataIteratorBase(nullptr)
	{
	}

	template <class TDatum>
	DataIterator<TDatum>::DataIterator(DataArray<TDatum> *p_Data)
		: DataIteratorBase(p_Data)
	{
	}

	template <class TDatum>
	TDatum *DataIterator<TDatum>::Next()
	{
		return static_cast<TDatum *>(DataIteratorBase::Next());
	}

	template <class TDatum>
	DataIterator<TDatum> &DataIterator<TDatum>::operator++()
	{
		Next();
		return *this;
	}

	template <class TDatum>
	DataIterator<TDatum> DataIterator<TDatum>::operator++(int32_t)
	{
		auto result = *this;
		operator++();
		return result;
	}

	template <class TDatum>
	TDatum *DataIterator<TDatum>::operator->() const
	{
		return static_cast<TDatum *>(Array->GetAddress(CurrentDatumIndex));
	}

	template <class TDatum>
	TDatum &DataIterator<TDatum>::operator*() const
	{
		return *operator->();
	}

	template <class TDatum>
	bool DataIterator<TDatum>::operator==(const DataIterator<TDatum> &p_Other) const
	{
		return Array == p_Other.Array
			&& CurrentIndex == p_Other.CurrentIndex
			&& CurrentDatumIndex == p_Other.CurrentDatumIndex;
	}

	template <class TDatum>
	bool DataIterator<TDatum>::operator!=(const DataIterator<TDatum> &p_Other) const
	{
		return !(*this == other);
	}
}