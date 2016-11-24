#pragma once
#include "../../Math/Bounds.hpp"
#include "../TagBlock.hpp"
#include "../TagData.hpp"
#include "../TagReference.hpp"
#include "../../Text/StringID.hpp"
#include "../Objects/Object.hpp"

namespace Blam::Tags::Items
{
	using Blam::Math::Bounds;
	using Blam::Tags::TagBlock;
	using Blam::Tags::TagReference;
	using Blam::Text::StringID;
	using Blam::Tags::Objects::Object;

	struct Item : Object
	{
		enum class Flags : int32_t;
		struct PredictedBitmap;

		Item::Flags ItemFlags : 32;
		short : 16; // old message index
		int16_t SortOrder;
		long : 32; // old multiplayer on-ground scale
		long : 32; // old single player on-ground scale
		StringID PickupMessage;
		StringID SwapMessage;
		StringID PickupOrDualWieldMessage;
		StringID SwapOrDualWieldMessage;
		StringID PickedUpMessage;
		StringID SwitchToMessage;
		StringID SwitchToFromAIMessage;
		StringID AllWeaponsEmptyMessage;
		TagReference CollisionSound;
		TagBlock<Item::PredictedBitmap> PredictedBitmaps;
		TagReference DetonationDamageEffect;
		Bounds<float> DetonationDelayBounds;
		TagReference DetonatingEffect;
		TagReference DetonationEffect;
		float GroundScaleSP;
		float GroundScaleMP;
		float SmallHoldScale;
		float SmallHolsterScale;
		float MediumHoldScale;
		float MediumHolsterScale;
		float LargeHoldScale;
		float LargeHolsterScale;
		float HugeHoldScale;
		float HugeHolsterScale;
		float GroundedFrictionLength;
		float GroundedFrictionUnknown;

		enum class Item::Flags : int32_t
		{
			None,
			AlwaysMaintainsZUp = 1 << 0,
			DestroyedByExplosions = 1 << 1,
			UnaffectedByGravity = 1 << 2
		};

		struct Item::PredictedBitmap
		{
			TagReference Bitmap;
		};
		static_assert(sizeof(Item::PredictedBitmap) == 0x10, "");
	};
	static_assert(sizeof(Item) == 0x1D4, "");
}