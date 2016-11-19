#include "ObjectDatum.hpp"

namespace Blam::Objects
{
	ObjectDatum::ObjectDatum()
		: Type(ObjectType::None), Data(nullptr)
	{
	}

	Data::DatumIndex ObjectDatum::GetTagIndex() const
	{
		if (!Data)
			return Data::DatumIndex::Null;

		return *static_cast<Data::DatumIndex *>(Data);
	}
}