#pragma once
#include <cstdint>
#include <iterator>
#include "DatumBase.hpp"
#include "DataIteratorBase.hpp"
#include "DataArray.hpp"

namespace Blam::Data
{
	template<class TDatum>
	struct DataIterator : DataIteratorBase, std::iterator<std::forward_iterator_tag, TDatum>
	{
		static_assert(std::is_base_of<DatumBase, TDatum>::value, "Blam::Data::DataIterator<TDatum>: TDatum must inherit from Blam::Data::DatumBase");

		DataIterator(DataArray<TDatum> *p_Data)
			: DataIteratorBase(p_Data)
		{
		}

		DataIterator()
			: DataIteratorBase(nullptr)
		{
		}

		TDatum *Next()
		{
			return static_cast<TDatum *>(DataIteratorBase::Next());
		}

		DataIterator<TDatum> &operator++()
		{
			Next();
			return *this;
		}

		DataIterator<TDatum> operator++(int32_t)
		{
			auto result = *this;
			operator++();
			return result;
		}

		TDatum *operator->() const
		{
			return static_cast<TDatum *>(Array->GetAddress(CurrentDatumIndex));
		}

		TDatum &operator*() const
		{
			return *operator->();
		}

		bool operator==(const DataIterator<TDatum> &p_Other) const
		{
			return Array == p_Other.Array
				&& CurrentIndex == p_Other.CurrentIndex
				&& CurrentDatumIndex == p_Other.CurrentDatumIndex;
		}

		bool operator!=(const DataIterator<TDatum> &p_Other) const
		{
			return !(*this == other);
		}
	};
	static_assert(sizeof(DataIterator<DatumBase>) == sizeof(DataIteratorBase), "Blam::Data::DataIterator<TDatum>: Invalid size.");
}