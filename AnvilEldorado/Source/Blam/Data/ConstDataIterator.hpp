#pragma once
#include <cstdint>
#include <iterator>
#include "DatumBase.hpp"
#include "DataIteratorBase.hpp"
#include "DataArray.hpp"

namespace Blam::Data
{
	template<class TDatum>
	struct ConstDataIterator : DataIteratorBase, std::iterator<std::forward_iterator_tag, TDatum>
	{
		static_assert(std::is_base_of<DatumBase, TDatum>::value, "Blam::Data::ConstDataIterator<TDatum>: TDatum must inherit from Blam::Data::DatumBase");

		ConstDataIterator(const DataArray<TDatum> *p_Data)
			: DataIteratorBase(p_Data)
		{
		}

		ConstDataIterator()
			: ConstDataIterator(nullptr)
		{
		}

		const TDatum* Next()
		{
			return static_cast<TDatum *>(DataIteratorBase::Next());
		}

		ConstDataIterator<TDatum> &operator++()
		{
			Next();
			return *this;
		}

		ConstDataIterator<TDatum> operator++(int32_t)
		{
			auto result = *this;
			operator++();
			return result;
		}

		const TDatum *operator->() const
		{
			return static_cast<TDatum *>(Array->GetAddress(CurrentDatumIndex));
		}

		const TDatum &operator*() const
		{
			return *operator->();
		}

		bool operator==(const ConstDataIterator<TDatum> &p_Other) const
		{
			return Array == p_Other.Array
				&& CurrentIndex == p_Other.CurrentIndex
				&& CurrentDatumIndex == p_Other.CurrentDatumIndex;
		}

		bool operator!=(const ConstDataIterator<TDatum> &p_Other) const
		{
			return !(*this == p_Other);
		}
	};
	static_assert(sizeof(ConstDataIterator<Data::DatumBase>) == sizeof(DataIteratorBase), "Blam::Data::ConstDataIterator<TDatum>");
}