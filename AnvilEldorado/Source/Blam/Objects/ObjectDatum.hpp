#pragma once
#include <cstdint>
#include "Globals.hpp"
#include "Blam\Data\DataArray.hpp"
#include "Blam\Data\DatumBase.hpp"
#include "Blam\Data\DatumIndex.hpp"
#include "ObjectType.hpp"

namespace Blam::Objects
{
	struct ObjectDatumBase : Data::DatumBase
	{
		uint8_t Flags;
		ObjectType Type : 8;
		uint32_t Unknown8;
		uint32_t PoolOffset;
		void *Data;

		ObjectDatumBase()
			: Type(ObjectType::None), Data(nullptr)
		{
		}

		static const Data::DataArray<ObjectDatumBase> *GetDataArray()
		{
			return AnvilCommon::GetThreadStorage<const Blam::Data::DataArray<ObjectDatumBase>>(0x448);
		}

		Data::DatumIndex GetTagIndex() const
		{
			if (!Data)
				return Data::DatumIndex::Null;

			return *static_cast<Data::DatumIndex *>(Data);
		}
	};
	static_assert(sizeof(ObjectDatumBase) == 0x10, "Blam::Objects::ObjectDatumBase");
}