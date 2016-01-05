#pragma once

namespace Network
{
	class network_observer
	{
	public:
		typedef void(__thiscall* channel_send_message_t)(network_observer* This, int p_OwnerIndex, int p_ChannelIndex, bool p_Secure, int p_Id, int p_PacketSize, const void* p_Data);
		static channel_send_message_t channel_send_message;

		typedef void*(__thiscall* observer_channel_send_message_t)(network_observer* This, void* v1, void* v2, void* v3);
		static observer_channel_send_message_t observer_channel_send_message;

		typedef bool(__thiscall* build_secure_connection_t)(network_observer* This, void* v1, void* v2, void* v3, void* v4);
		static build_secure_connection_t build_secure_connection;
	};
}