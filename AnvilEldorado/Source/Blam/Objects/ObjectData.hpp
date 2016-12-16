#pragma once
#include <cstdint>
#include "Globals.hpp"
#include "Blam\Data\DataArray.hpp"
#include "Blam\Data\DatumBase.hpp"
#include "Blam\Data\DatumIndex.hpp"
#include "ObjectType.hpp"

namespace Blam::Objects
{
	struct ObjectData : Data::DatumBase
	{
		uint8_t Flags;
		ObjectType Type : 8;
		uint32_t Unknown8;
		uint32_t PoolOffset;
		void *Data;

		ObjectData()
			: Type(ObjectType::None), Data(nullptr)
		{
		}

		static const Data::DataArray<ObjectData> *GetDataArray()
		{
			return AnvilCommon::GetThreadStorage<const Blam::Data::DataArray<ObjectData>>(0x448);
		}

		Data::DatumIndex GetTagIndex() const
		{
			using Blam::Data::DatumIndex;

			if (!Data)
				return DatumIndex::Null;

			return *static_cast<DatumIndex *>(Data);
		}
	};
	static_assert(sizeof(ObjectData) == 0x10, "Blam::Objects::ObjectData");
}