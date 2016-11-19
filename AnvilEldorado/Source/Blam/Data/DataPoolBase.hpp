#pragma once
#include <cstdint>

namespace Blam::Data
{
	struct DataPoolBase
	{
		int32_t Signature;
		char Name[0x20];
		void** Allocator;
		int32_t Size;
		int32_t FreeSize;
		int32_t Padding;
		int32_t Unknown52;
		int32_t Unknown56;
		uint16_t Unknown60;
		uint8_t Unknown62;
		uint8_t Unknown63;
		int32_t Unknown64;

		DataPoolBase();
	};
	static_assert(sizeof(DataPoolBase) == 0x44, "Blam::Data::DataPoolBase");
}