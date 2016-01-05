#pragma once

namespace Network
{
	namespace Packets
	{
		class packets
		{
		public:

			typedef bool(__fastcall* init_packet_t)(void* v0, void* v1);
			static init_packet_t init_packet;
		};
	}
}