#include "GlobalMemoryMap.hpp"

namespace Blam::Memory
{
	GlobalMemoryMapStage::GlobalMemoryMapStage()
		: DataAddress(nullptr), CacheAddress(nullptr)
	{
	}

	GlobalMemoryMap::GlobalMemoryMap()
		: Allocator(nullptr), DataBaseAddress(nullptr), CacheBaseAddress(nullptr)
	{
	}

	GlobalMemoryMap* GlobalMemoryMap::Instance()
	{
		return reinterpret_cast<GlobalMemoryMap *>(0x238EC50);
	}
}