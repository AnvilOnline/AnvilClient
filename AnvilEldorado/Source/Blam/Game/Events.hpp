#pragma once
#include <cstdint>
#include "..\Data\DatumIndex.hpp"
#include "..\Tags\TagReference.hpp"

namespace Blam::Game
{
	enum class EventType : int16_t
	{
		General,
		Flavor,
		Slayer,
		Ctf,
		Oddball,
		Forge,
		Koth,
		Vip,
		Juggernaut,
		Territories,
		Assault,
		Infection,
		Survival,
		Wp
	};

	enum class EventAudience : int16_t
	{
		CausePlayer,
		CauseTeam,
		EffectPlayer,
		EffectTeam,
		All
	};

	struct Event
	{
		EventType Type : 16;
		uint32_t NameStringID;
		int32_t Unknown8;
		int32_t UnknownC;
		Data::DatumIndex CausePlayer;
		int32_t CauseTeam;
		Data::DatumIndex EffectPlayer;
		int32_t EffectTeam;
		int32_t Unknown20;
		int16_t Unknown24;
	};
	static_assert(sizeof(Event) == 0x28, "Blam::Game::Event");

	struct EventDefinition
	{
		uint16_t Flags;
		EventType Type : 16;
		uint32_t NameStringID;
		EventAudience Audience;
		uint16_t UnknownA;
		int16_t Team;
		uint16_t UnknownE;
		uint32_t MessageStringID;
		uint32_t MedalStringID;
		uint32_t Unknown18;
		uint32_t Unknown1C;
		uint16_t RequiredField;
		uint16_t ExcludedAudience;
		uint16_t RequiredField2;
		uint16_t ExcludedAudience2;
		uint32_t PrimaryStringId;
		int32_t PrimaryStringDuration;
		uint32_t PluralDisplayStringId;
		float SoundDelay;
		uint16_t SoundFlags;
		uint16_t Unknown3A;
		Tags::TagReference LanguageSounds[12];
		uint32_t UnknownFC;
		uint32_t Unknown100;
		uint32_t Unknown104;
		uint32_t Unknown108;
	};
	static_assert(sizeof(EventDefinition) == 0x10C, "Blam::Game::EventDefinition");
}