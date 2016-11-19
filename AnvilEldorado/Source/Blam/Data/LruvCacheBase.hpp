#pragma once
#include <cstdint>

namespace Blam::Data
{
	struct LruvCacheBase
	{
		char Name[0x20];
		void *Unknown32;
		void *Unknown36;
		void *Unknown40;
		void *Unknown44;
		int32_t Unknown48;
		int32_t Unknown52;
		int32_t Unknown56;
		int32_t Unknown60;
		int32_t Unknown64;
		int32_t Unknown68;
		int32_t Unknown72;
		int32_t Unknown76;
		int32_t Unknown80;
		int32_t Unknown84;
		int32_t Unknown88;
		int32_t Unknown92;
		int32_t Unknown96;
		int32_t Unknown100;
		int32_t Unknown104;
		int32_t Unknown108;
		int32_t Unknown112;
		int32_t Signature;
		void **Allocator;
		int32_t Unknown124;
		int32_t Unknown128;

		LruvCacheBase();
	};
	static_assert(sizeof(LruvCacheBase) == 0x84, "Blam::Data::LruvCacheBase");
}