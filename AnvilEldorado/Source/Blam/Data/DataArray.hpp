#pragma once
#include <iterator>
#include <type_traits>
#include "DatumIndex.hpp"
#include "DatumBase.hpp"

namespace Blam::Data
{
	struct DataArrayBase
	{
		char Name[0x20];
		int32_t MaxCount;
		int32_t DatumSize;
		uint8_t Alignment;
		bool IsValid;
		uint16_t Flags;
		int32_t Signature;
		void *Allocator;
		int32_t NextIndex;
		int32_t FirstUnallocated;
		int32_t ActualCount;
		uint16_t NextSalt;
		uint16_t AltNextSalt;
		void *Data;
		uint32_t *ActiveIndices;
		int32_t HeaderSize;
		int32_t TotalSize;

		DataArrayBase();

		DatumBase *Get(const DatumIndex &p_Index) const;
		DatumBase *GetAddress(const DatumIndex &p_Index) const;
	};
	static_assert(sizeof(DataArrayBase) == 0x54, "Blam::Data::DataArrayBase");

	struct DataIteratorBase
	{
		const DataArrayBase *Array;
		DatumIndex CurrentDatumIndex;
		int32_t CurrentIndex;

		// Creates a data iterator for an array.
		explicit DataIteratorBase(const DataArrayBase *data) : Array(data), CurrentDatumIndex(DatumIndex::Null), CurrentIndex(-1) { }

		DatumBase *Next();
	};
	static_assert(sizeof(DataIteratorBase) == 0xC, "Blam::Data::DataIteratorBase");

	template<class TDatum> struct DataIterator;
	template<class TDatum> struct ConstDataIterator;

	// Type-safe data array struct.
	// TDatum MUST inherit from the DatumBase struct.
	template<class TDatum>
	struct DataArray : DataArrayBase
	{
		static_assert(std::is_base_of<DatumBase, TDatum>::value, "TDatum must inherit from DatumBase");

		// Gets a pointer to the datum corresponding to a datum index.
		// Returns null if the datum index does not match a valid datum.
		TDatum* Get(DatumIndex index) const { return static_cast<TDatum*>(DataArrayBase::Get(index)); }

		// Gets a reference to the datum corresponding to a datum index.
		// The datum index is NOT checked for validity and this will always succeed.
		// Use Get() if you need validity checking.
		TDatum& operator[](DatumIndex index) const { return *static_cast<TDatum*>(GetAddress(index)); }

		// Gets an iterator pointing to the beginning of this data array.
		DataIterator<TDatum> begin()
		{
			DataIterator<TDatum> result(this);
			result.Next();
			return result;
		}

		// Gets a const iterator pointing to the beginning of this data array.
		ConstDataIterator<TDatum> begin() const
		{
			ConstDataIterator<TDatum> result(this);
			result.Next();
			return result;
		}

		// Gets a const iterator pointing to the beginning of this data array.
		ConstDataIterator<TDatum> cbegin() const { return begin(); }

		// Gets an iterator pointing to the end of this data array.
		DataIterator<TDatum> end()
		{
			DataIterator<TDatum> result(this);
			result.CurrentIndex = MaxCount;
			return result;
		}

		// Gets a const iterator pointing to the end of this data array.
		ConstDataIterator<TDatum> end() const
		{
			ConstDataIterator<TDatum> result(this);
			result.CurrentIndex = MaxCount;
			return result;
		}

		// Gets a const iterator pointing to the end of this data array.
		ConstDataIterator<TDatum> cend() const { return end(); }
	};
	static_assert(sizeof(DataArray<DatumBase>) == sizeof(DataArrayBase), "Invalid DataArray size");

	// Type-safe struct for a forward iterator which iterates over the values in a DataArray.
	template<class TDatum>
	struct DataIterator : DataIteratorBase, std::iterator<std::forward_iterator_tag, TDatum>
	{
		static_assert(std::is_base_of<DatumBase, TDatum>::value, "TDatum must inherit from DatumBase");

		// Creates a data iterator for an array.
		explicit DataIterator(DataArray<TDatum> *data) : DataIteratorBase(data) { }

		// Moves to the next datum and returns a pointer to it.
		// Returns null if at the end of the array.
		TDatum* Next() { return static_cast<TDatum*>(DataIteratorBase::Next()); }

		DataIterator() : DataIteratorBase(nullptr) { }
		DataIterator<TDatum>& operator++() { Next(); return *this; }
		DataIterator<TDatum> operator++(int) { auto result = *this; operator++(); return result; }
		TDatum* operator->() const { return static_cast<TDatum*>(Array->GetAddress(CurrentDatumIndex)); }
		TDatum& operator*() const { return *operator->(); }
		bool operator==(const DataIterator<TDatum> &rhs) const { return Array == rhs.Array && CurrentIndex == rhs.CurrentIndex && CurrentDatumIndex == rhs.CurrentDatumIndex; }
		bool operator!=(const DataIterator<TDatum> &rhs) const { return !(*this == rhs); }
	};
	static_assert(sizeof(DataIterator<DatumBase>) == sizeof(DataIteratorBase), "Invalid DataIterator size");

	// Type-safe struct for a const forward iterator which iterates over the values in a DataArray.
	template<class TDatum>
	struct ConstDataIterator : DataIteratorBase, std::iterator<std::forward_iterator_tag, TDatum>
	{
		static_assert(std::is_base_of<DatumBase, TDatum>::value, "TDatum must inherit from DatumBase");

		// Creates a const data iterator for an array.
		explicit ConstDataIterator(const DataArray<TDatum> *data) : DataIteratorBase(data) { }

		// Moves to the next datum and returns a pointer to it.
		// Returns null if at the end of the array.
		const TDatum* Next() { return static_cast<TDatum*>(DataIteratorBase::Next()); }

		ConstDataIterator() : DataIteratorBase(nullptr) { }
		ConstDataIterator<TDatum>& operator++() { Next(); return *this; }
		ConstDataIterator<TDatum> operator++(int) { auto result = *this; operator++(); return result; }
		const TDatum* operator->() const { return static_cast<TDatum*>(Array->GetAddress(CurrentDatumIndex)); }
		const TDatum& operator*() const { return *operator->(); }
		bool operator==(const ConstDataIterator<TDatum> &rhs) const { return Array == rhs.Array && CurrentIndex == rhs.CurrentIndex && CurrentDatumIndex == rhs.CurrentDatumIndex; }
		bool operator!=(const ConstDataIterator<TDatum> &rhs) const { return !(*this == rhs); }
	};
	static_assert(sizeof(ConstDataIterator<DatumBase>) == sizeof(DataIteratorBase), "Invalid ConstDataIterator size");
}