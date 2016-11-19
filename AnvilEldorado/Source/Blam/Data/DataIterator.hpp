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

		DataIterator();
		explicit DataIterator(DataArray<TDatum> *p_Data);

		TDatum *Next();

		DataIterator<TDatum> &operator++();
		DataIterator<TDatum> operator++(int32_t);

		TDatum *operator->() const;

		TDatum &operator*() const;

		bool operator==(const DataIterator<TDatum> &p_Other) const;
		bool operator!=(const DataIterator<TDatum> &p_Other) const;
	};
	static_assert(sizeof(DataIterator<DatumBase>) == sizeof(DataIteratorBase), "Blam::Data::DataIterator<TDatum>: Invalid size.");
}