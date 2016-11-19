#pragma once
#include <cstdint>
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
}