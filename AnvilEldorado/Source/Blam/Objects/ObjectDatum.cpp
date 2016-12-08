#include "Globals.hpp"
#include "Blam\Data\DataArray.hpp"
#include "ObjectDatum.hpp"

namespace Blam::Objects
{
	ObjectDatum::ObjectDatum()
		: Type(ObjectType::None), Data(nullptr)
	{
	}

	const Data::DataArray<ObjectDatum> *ObjectDatum::GetDataArray()
	{
		return AnvilCommon::GetThreadStorage<const Blam::Data::DataArray<Blam::Objects::ObjectDatum>>(0x448);
	}

	Data::DatumIndex ObjectDatum::GetTagIndex() const
	{
		if (!Data)
			return Data::DatumIndex::Null;

		return *static_cast<Data::DatumIndex *>(Data);
	}
}