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

		ConstDataIterator();
		explicit ConstDataIterator(const DataArray<TDatum> *data);

		const TDatum* Next();

		ConstDataIterator<TDatum> &operator++();
		ConstDataIterator<TDatum> operator++(int32_t);

		const TDatum *operator->() const;

		const TDatum &operator*() const;

		bool operator==(const ConstDataIterator<TDatum> &rhs) const;
		bool operator!=(const ConstDataIterator<TDatum> &rhs) const;
	};
	static_assert(sizeof(ConstDataIterator<Data::DatumBase>) == sizeof(DataIteratorBase), "Blam::Data::ConstDataIterator<TDatum>");
}