#pragma once
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