#pragma once
#include <cstdint>

namespace Blam::Tags
{
	struct TagInstance
	{
		uint16_t Index;

		TagInstance(const uint16_t p_Index) : Index(p_Index) {}
		TagInstance() : TagInstance(0xFFFF) {}

		uint32_t GetGroupTag() { return reinterpret_cast<int(*)(uint16_t)>(0x5033A0)(Index); }

		template <typename T>
		T *GetDefinition()
		{
			if (Index == 0xFFFF)
				return nullptr;
			
			return reinterpret_cast<T *>(reinterpret_cast<void *(*)(int32_t, uint32_t index)>(0x503370)(GetGroupTag(), Index));
		}
	};
}