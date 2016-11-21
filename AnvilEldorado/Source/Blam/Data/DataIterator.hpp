#pragma once
#include <cstdint>
#include <iterator>
#include "DatumBase.hpp"
#include "DataArray.hpp"

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
	static_assert(sizeof(DataIterator<DatumBase>) == sizeof(DataIteratorBase), "Blam::Data::DataIterator<TDatum>");

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