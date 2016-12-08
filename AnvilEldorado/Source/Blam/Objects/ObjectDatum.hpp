#pragma once
#include <cstdint>
#include "..\Data\DatumIndex.hpp"
#include "..\Data\DatumBase.hpp"

namespace Blam::Objects
{
	enum class ObjectType : int8_t
	{
		None = -1,
		Biped,
		Vehicle,
		Weapon,
		Equipment,
		ArgDevice,
		Terminal,
		Projectile,
		Scenery,
		Machine,
		Control,
		SoundScenery,
		Crate,
		Creature,
		Giant,
		EffectScenery
	};
	static_assert(sizeof(ObjectType) == 0x1, "Blam::Objects::ObjectType");

	struct ObjectDatum : Data::DatumBase
	{
		uint8_t Flags;
		ObjectType Type : 8;
		uint32_t Unknown8;
		uint32_t PoolOffset;
		void *Data;

		ObjectDatum();

		static const Blam::Data::DataArray<ObjectDatum> *GetDataArray();

		Data::DatumIndex GetTagIndex() const;
	};
	static_assert(sizeof(ObjectDatum) == 0x10, "Blam::Objects::ObjectDatum");
}