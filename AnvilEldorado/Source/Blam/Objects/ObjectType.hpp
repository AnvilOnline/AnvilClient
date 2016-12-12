#pragma once
#include <cstdint>

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
}