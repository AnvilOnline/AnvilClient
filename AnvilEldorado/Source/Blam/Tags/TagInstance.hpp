#pragma once
#include <cstdint>
#include "Blam\Data\DatumIndex.hpp"
#include "Blam\Tags\Tag.hpp"

namespace Blam::Tags
{
	struct TagInstance
	{
		Blam::Data::DatumIndex Index;

		TagInstance(const Blam::Data::DatumIndex &p_Index) : Index(p_Index) {}
		TagInstance(const uint16_t p_Index) : TagInstance(Blam::Data::DatumIndex(0, p_Index)) {}
		TagInstance() : TagInstance(Blam::Data::DatumIndex::Null) {}

		Tag GetGroupTag() const { return reinterpret_cast<Tag(*)(uint16_t)>(0x5033A0)(Index.Index); }

		template <typename T>
		T *GetDefinition()
		{
			using Blam::Data::DatumIndex;

			if (Index == DatumIndex::Null)
				return nullptr;
			
			return reinterpret_cast<T *>(reinterpret_cast<void *(*)(Tag, DatumIndex)>(0x503370)(GetGroupTag(), Index));
		}
	};
}