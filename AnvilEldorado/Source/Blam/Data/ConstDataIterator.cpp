#include "ConstDataIterator.hpp"

namespace Blam::Data
{
	template <class TDatum>
	ConstDataIterator<TDatum>::ConstDataIterator()
		: ConstDataIterator(nullptr)
	{
	}

	template <class TDatum>
	ConstDataIterator<TDatum>::ConstDataIterator(const DataArray<TDatum> *p_Data)
		: DataIteratorBase(p_Data)
	{
	}

	template <class TDatum>
	const TDatum* ConstDataIterator<TDatum>::Next()
	{
		return static_cast<TDatum *>(DataIteratorBase::Next());
	}

	template <class TDatum>
	ConstDataIterator<TDatum> &ConstDataIterator<TDatum>::operator++()
	{
		Next();
		return *this;
	}

	template <class TDatum>
	ConstDataIterator<TDatum> ConstDataIterator<TDatum>::operator++(int32_t)
	{
		auto result = *this;
		operator++();
		return result;
	}

	template <class TDatum>
	const TDatum *ConstDataIterator<TDatum>::operator->() const
	{
		return static_cast<TDatum *>(Array->GetAddress(CurrentDatumIndex));
	}

	template <class TDatum>
	const TDatum &ConstDataIterator<TDatum>::operator*() const
	{
		return *operator->();
	}

	template <class TDatum>
	bool ConstDataIterator<TDatum>::operator==(const ConstDataIterator<TDatum> &p_Other) const
	{
		return Array == p_Other.Array
			&& CurrentIndex == p_Other.CurrentIndex
			&& CurrentDatumIndex == p_Other.CurrentDatumIndex;
	}

	template <class TDatum>
	bool ConstDataIterator<TDatum>::operator!=(const ConstDataIterator<TDatum> &p_Other) const
	{
		return !(*this == p_Other);
	}
}