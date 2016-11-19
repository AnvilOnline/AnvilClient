#pragma once
#include <cstdint>

namespace Blam::Memory
{
	struct GlobalMemoryMapStage
	{
		char* DataAddress;
		char* CacheAddress;
		uint32_t IsInitialized;
		uint32_t DataAllocationCount;
		uint32_t CacheAllocationCount;

		GlobalMemoryMapStage();
	};

	struct GlobalMemoryMap
	{
		void** Allocator;
		uint32_t CurrentStageIndex;
		char* DataBaseAddress;
		char* CacheBaseAddress;
		uint32_t IsInitialized;
		GlobalMemoryMapStage Stages[8];

		GlobalMemoryMap();

		static GlobalMemoryMap *Instance();
	};
	static_assert(sizeof(GlobalMemoryMap) == 0xB4, "Blam::Memory::GlobalMemoryMap");
}